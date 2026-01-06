//
// Created by david on 12/10/25.
//

#ifndef HALAL_NSGAIISOLVER_HPP
#define HALAL_NSGAIISOLVER_HPP
#include <algorithm>
#include <random>
#include <set>
#include <utility>

#include "../interfaces-and-templates/Chromosome.h"
#include "../interfaces-and-templates/OptimizationProblem.h"
#include "ranges_ext.hpp"
#include "../interfaces-and-templates/Solution.h"
#include "../interfaces-and-templates/StopCondition.hpp"

namespace ranges = std::ranges;

template<ranges::forward_range R, typename T = ranges::range_value_t<R>>
requires ParetoSolution<T>
void NonDominatedSort(R& elements, IParetoDominanceComparator<T>& comparator)
{
    std::vector<int> pareto_front;
    DynamicMatrix<int> dominated_matrix(elements.size());
    for (int i = 0; i < elements.size(); ++i) {
        auto p = elements.at(i);
        for (int j = 0; j < elements.size(); ++j) {
            if (elements.at(i) == elements.at(j)) continue;
            if (comparator(*elements.at(i), *elements.at(j))) {
                ++p->dominated_by;
                dominated_matrix.PushBack(j, i);
            }
        }
        if (p->dominated_by == 0) {
            pareto_front.push_back(i);
        }
    }
    auto pfi = 1;
    while (!pareto_front.empty()) {
        std::vector<int> next_pareto_front;
        for (const auto p_idx : pareto_front) {
            elements.at(p_idx)->pareto_rank = pfi;
            for (const auto q_idx : dominated_matrix.Row(p_idx)) {
                auto q = elements.at(q_idx);
                --q->dominated_by;
                if (q->dominated_by == 0) {
                    next_pareto_front.push_back(q_idx);
                }
            }
        }
        ++pfi;
        pareto_front = next_pareto_front;
    }
    std::sort(elements.begin(), elements.end(), [](const auto lhs, const auto rhs) -> bool
    {
        return lhs.pareto_rank < rhs.pareto_rank;
    });
}

template <ranges::input_range R, ranges::forward_range RG,
    typename T = ranges::range_value_t<R>,
    typename IterT = ranges::iterator_t<R>,
    typename N = T::NumberType>
requires ParetoSolution<T>
auto CrowdingDistance(R& elements,
                      RG& objectives) -> std::vector<std::pair<IterT, N>>
{
    std::vector<std::pair<IterT, N>> crowding_distances;
    for (auto it = ranges::begin(elements); it != ranges::end(elements); ++it) {
        crowding_distances.emplace_back(std::make_pair(it, N{0}));
    }
    for (auto func : objectives) {
        ranges::sort(crowding_distances, [func](auto lhs, auto rhs) -> bool
        {
            return func(*lhs.first) < func(*rhs.first);
        });
        auto [g_min, g_max] = ranges::minmax(ranges::views::transform(crowding_distances, [func](const auto& x) -> auto
        {
            return func(*x.first);
        }));
        ranges::begin(crowding_distances)->second = std::numeric_limits<N>::infinity();
        ranges::rbegin(crowding_distances)->second = std::numeric_limits<N>::infinity();
        for (auto it = ranges::begin(crowding_distances) + 1; it != ranges::end(crowding_distances) - 1; ++it) {
            it->second += (func(*(it+1)->first) - func(*(it-1)->first)) / (g_max - g_min);
        }
    }
    return crowding_distances;
}

template<ranges::forward_range R, ranges::forward_range RG, typename T = ranges::range_value_t<R>>
requires ParetoSolution<T>
auto NSGAIISelection(R& population, R& next_gen, RG& objectives, const int population_size,
                     IParetoDominanceComparator<T>& comparator)
{
    std::vector<T> selected;
    auto all_elements = concat_view(population, next_gen);
    NonDominatedSort(all_elements, comparator);
    auto pfi = 1;
    auto n_selected = selected.size();
    while (n_selected < population_size) {
        auto pareto_front = ranges::views::filter(all_elements, [pfi](auto e) -> bool
        {
            return e.pareto_rank == pfi;
        });
        //if (pareto_front.empty()) break;
        if (ranges::distance(pareto_front) + ranges::distance(selected) <= population_size) {
            ranges::copy(pareto_front, std::back_inserter(selected));
            n_selected = selected.size();
        } else {
            auto elems_with_dist = CrowdingDistance(pareto_front, objectives);
            ranges::sort(elems_with_dist, [](const auto& p, const auto& q) -> bool
            {
                return p.second < q.second;
            });
            auto rest = population_size - selected.size();
            auto n_elems = elems_with_dist | std::views::take(rest) | std::views::transform([](auto elem) -> auto
            {
                return *elem.first;
            });
            ranges::copy(n_elems, std::back_inserter(selected));
            n_selected = selected.size();
        }
        ++pfi;
    }
    return selected;
}


template<class P, StopConditionFunctor S, class C, typename T = P::SolutionType>
requires std::derived_from<P, Chromosome<T>> && std::derived_from<C, IParetoDominanceComparator<T>>
auto NSGAII(
    const P* problem,
    const size_t population_size,
    const int parent_pool_size,
    const int n_offspring,
    C& comparator,
    const int n_parents,
    const float mutation_rate,
    S& stop_condition)
{
    std::vector<T> population;
    population.reserve(population_size);
    std::vector<T> new_gen;
    new_gen.reserve(n_offspring);
    std::set<T> best;
    std::mt19937 gen{std::random_device{}()};
    for (int i = 0; std::cmp_less(i , population_size); ++i) {
        population.push_back(problem->GenerateInstance());
    }
    while (!StopCondition(stop_condition, 0.0f)) {
        auto objectives = problem->GetObjectives();
        population = NSGAIISelection(population, new_gen, objectives, population_size, comparator);
        population.erase(population.begin() + population_size, population.end());
        auto first_front = std::views::filter(population, [](const auto& p) -> bool
        {
            return p.pareto_rank == 1;
        });
        best.insert(first_front.begin(), first_front.end());

        auto potential_parents = sample_view(population, parent_pool_size, gen);
        std::uniform_real_distribution real_dist;
        std::uniform_int_distribution<size_t> int_dist{0, potential_parents.size() - 1};
        std::vector<T> new_gen_temp;
        new_gen_temp.reserve(n_offspring);
        for (int i = 0; i < n_offspring; ++i) {
            std::vector<T> parents;
            for (int j = 0; j < n_parents; ++j) {
                parents.push_back(*potential_parents.at(int_dist(gen)));
            }
            auto child = problem->CrossOver(parents);
            if (real_dist(gen) < mutation_rate)
                problem->Mutate(child);
            new_gen_temp.push_back(child);
            new_gen = new_gen_temp;
        }
    }
    return best;
}


#endif //HALAL_NSGAIISOLVER_HPP