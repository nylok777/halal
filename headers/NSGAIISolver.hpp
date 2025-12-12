//
// Created by david on 12/10/25.
//

#ifndef HALAL_NSGAIISOLVER_HPP
#define HALAL_NSGAIISOLVER_HPP
#include <algorithm>
#include <memory>
#include <random>
#include <set>
#include <unordered_map>

#include "Chromosome.h"
#include "ProblemRepresentation.h"
#include "RangesExt.h"
#include "Solution.h"
#include "StopCondition.h"

namespace ranges = std::ranges;

template<ranges::input_range R>
requires ranges::forward_range<R> && ParetoSolution<ranges::range_value_t<R>>
void NonDominatedSort(R&& elements)
{
    using iter = ranges::iterator_t<R>;
    std::vector<iter> pareto_front;
    std::unordered_map<iter, std::vector<iter>> dominated_elements;
    std::ranges::for_each(elements, [&dominated_elements](auto& x){dominated_elements.emplace(&x, {});});
    for (auto p = ranges::begin(elements); p != ranges::end(elements); ++p) {
        auto others = std::views::filter(elements ,[&p](auto& x){return &x != &p;});
        for (auto q = ranges::begin(others); q != ranges::end(others); ++q) {
            if (IsParetoDominatedBy(p, q)) {
                ++p.dominated_by;
                dominated_elements.at(q).push_back(p);
            }
        }
        if (p.dominated_by == 0) {
            pareto_front.push_back(p);
        }
    }
    auto pfi = 1;
    while (pareto_front.size() != 0) {
        std::vector<iter> F_next;
        for (auto p = ranges::begin(pareto_front); p != ranges::end(pareto_front); ++p) {
            p->pareto_rank = pfi;
            for (auto q = std::begin(dominated_elements.at(p)); q != std::end(dominated_elements.at(p)); ++q) {
                --q->dominated_by;
                if (q->dominated_by == 0) {
                    F_next.push_back(q);
                }
            }
        }
        ++pfi;
        pareto_front = F_next;
    }
}

template<ranges::forward_range R, ranges::forward_range RG>
requires ParetoSolution<ranges::range_value_t<R>> && std::is_function_v<ranges::range_value_t<RG>>
auto CrowdingDistance(R&& elements, RG&& objectives) -> std::vector<std::pair<ranges::iterator_t<R>, typename ranges::range_value_t<R>::NumberType>>
{
    using numeric_type = ranges::range_value_t<R>::NumberType;
    std::vector<std::pair<ranges::iterator_t<R>, numeric_type>> crowding_distances;
    for (auto it = std::ranges::begin(elements); it != ranges::end(elements); ++it) {
        crowding_distances.emplace_back(std::make_pair(it, numeric_type{0}));
    }
    for (auto func = ranges::begin(objectives); func != ranges::end(objectives); ++func) {
        ranges::sort(crowding_distances, [&func](const auto& lhs, const auto& rhs)
        {
            return func(lhs->first) < func(rhs->first);
        });
        auto [g_min, g_max] = ranges::minmax(ranges::views::transform(crowding_distances, [&func](const auto& x)
        {
            return func(x->first);
        }));
        ranges::begin(crowding_distances)->second = std::numeric_limits<numeric_type>::infinity();
        ranges::rbegin(crowding_distances)->second = std::numeric_limits<numeric_type>::infinity();
        for (auto it = ranges::begin(crowding_distances) + 1; it != ranges::end(crowding_distances) - 1; ++it) {
            it->second += (func((it+1)->first) - func((it-1)->first)) / (g_max - g_min);
        }
    }
    return crowding_distances;
}

template<ranges::forward_range R1, ranges::forward_range R2, ranges::forward_range RG>
requires ParetoSolution<ranges::range_value_t<R1>> && ParetoSolution<ranges::range_value_t<R2>> &&
    std::same_as<ranges::range_value_t<R1>, ranges::range_value_t<R2>>
auto NSGAIISelection(R1&& population, R2&& next_gen, RG&& objectives, const int n_best) -> std::set<ranges::range_value_t<R1>>
{
    std::set<ranges::range_value_t<R1>> selected{};
    auto all_elements = concat_view(population, next_gen);
    NonDominatedSort(all_elements);
    auto pfi = 1;
    auto n_selected = selected.size();
    while (n_selected < n_best) {
        auto pareto_front = all_elements | std::views::filter([pfi](auto e) { return e->pareto_rank == pfi; });
        if (ranges::size(pareto_front) + ranges::size(selected) <= n_best) {
            selected.insert(pareto_front.begin(), pareto_front.end());
            n_selected = selected.size();
        } else {
            auto elems_with_dist = CrowdingDistance(pareto_front, objectives);
            ranges::sort(elems_with_dist, [](const auto& p, const auto& q) { return p.second < q.second; });
            for (auto i = 0; i < elems_with_dist.size(); ++i) {
                selected.insert(elems_with_dist.at(i).first);
            }
        }
        ++pfi;
    }
    return selected;
}

template<ParetoSolution T, StopConditionFunctor S, class P>
requires std::derived_from<P, ParetoOptimizationProblem<T, typename T::NumberType>> &&
    std::derived_from<P, Chromosome<T>>
class NSGAIISolver
{
public:
    explicit NSGAIISolver(S&& stop_condition) : stop_condition(stop_condition) {}

    std::vector<T> operator()(
        P& problem,
        const int parent_pool_size,
        const int n_best,
        const int n_offspring,
        const float mutation_rate)
    {
        using iter_t = std::set<T>::const_iterator;
        std::set<T> population;
        std::set<T> new_gen;
        std::set<T> best;
        std::mt19937 gen{std::random_device{}()};
        std::ranges::generate(std::back_inserter(population), problem->GenerateInstance());
        while (StopCondition(stop_condition, 0.0f)) {
            population = NSGAIISelection(population, new_gen, n_best);
            std::ranges::copy_if(population, std::back_inserter(best), [](const auto& p){return p.pareto_rank == 1;});
            std::vector<iter_t> potential_parents; potential_parents.reserve(parent_pool_size);
            ranges::sample(population, potential_parents, parent_pool_size, gen);
            std::uniform_real_distribution<float> dist{0.0f, 1.0f};
            for (int i = 0; i < n_offspring; ++i) {
                std::vector<T> parents;
                ranges::sample(population, parents, 2, gen);
                auto child = problem.CrossOver(parents);
                if (dist(gen) < mutation_rate)
                    problem.Mutate(child);
                new_gen.insert(child);
            }
        }
        return best;
    }
private:
    S stop_condition;
};

#endif //HALAL_NSGAIISOLVER_HPP