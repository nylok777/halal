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

#include "GeneticSolvable.h"
#include "ProblemRepresentation.h"
#include "SolutionCandidate.h"
#include "StopCondition.h"


template<HasParetoRank T, StopConditionFunctor S, typename P, Numeric N>
requires std::derived_from<P, ParetoOptimizationProblem<T, N>>
class NSGAIISolver
{
public:
    using t_with_distance = std::pair<T, N>;
    explicit NSGAIISolver(P&& problem, const int parent_pool_size, const int new_gen_size)
        : problem(std::make_unique<P>(problem)), parent_pool_size(parent_pool_size), new_gen_size(new_gen_size) {}

    std::vector<T> operator()(S& stop_condition, const int n_best)
    {
        std::set<T> population;
        std::set<T> new_gen;
        std::set<T> best;
        std::ranges::generate(std::back_inserter(population), problem->GenerateInstance());
        while (StopCondition(stop_condition, 0.0f)) {
            auto selected_population = Selection(population, new_gen, n_best);
            std::ranges::copy_if(selected_population, std::back_inserter(best), [](const auto& p){return p.pareto_rank == 1;});
            new_gen = MakeNewPopulation(population);
        }
        return best;
    }
private:
    std::unique_ptr<P> problem;
    int parent_pool_size;
    int new_gen_size;

    std::set<T> MakeNewPopulation(std::set<T>& population)
    {
        std::vector<T> parents;
        std::set<T> children;
        std::ranges::sample(population, std::back_inserter(parents), parent_pool_size, std::mt19937{std::random_device{}()});
        for (int i = 1; i < new_gen_size - 1; ++i) {
            if (i < parents.size()) {
                children.emplace(CrossOver(parents.at(i-1), parents.at(i)));
            }
            else {
                children.emplace(CrossOver(parents.front(), parents.back()));
            }
        }
        return children;
    }

    std::set<t_with_distance> CrowdingDistance(std::set<T>& F)
    {
        std::vector<t_with_distance> D; D.reserve(F.size());
        std::ranges::views::transform(F, std::back_inserter(D),[](auto& x){ return t_with_distance{x, 0}; });
        for (auto& func : problem->GetObjectives()) {
            std::vector<N> func_results; func_results.reserve(D.size());
            std::ranges::transform(D, std::back_inserter(func_results), [&func](auto& p){return func(p.first);});
            std::ranges::sort(D, [&func](auto& p, auto& q){return func(p.first) < func(q.first);});
            D.at(0).second, D.back().second = std::numeric_limits<N>::max();
            for (int i = 1; i < D.size() - 1; ++i) {
                D.at(i).second += (func(D.at(i+1).first) - func(D.at(i-1).first)) /
                    (std::ranges::max(func_results) - std::ranges::min(func_results));
            }
        }
        return std::set<t_with_distance>{D.begin(), D.end()};
    }

    std::set<T> Selection(std::set<T>& population, std::set<T>& new_gen, const int n_best)
    {
        std::set<T> best;
        std::vector<T> R; R.reserve(population.size() + new_gen.size());
        std::ranges::set_union(population, new_gen, std::back_inserter(R));
        NonDominatedSort(R);
        auto pfi = 1;
        while (best.size() < n_best) {
            std::vector<T> F;
            std::ranges::copy_if(R, std::back_inserter(F), [pfi](const auto& p){return p.pareto_rank == pfi;});
            if (best.size() + F.size() <= n_best) {
                std::ranges::copy(F, std::back_inserter(best));
            }
            else {
                auto with_dist = CrowdingDistance(F);
                std::ranges::sort(with_dist, [](const auto& a, const auto& b){return a.second < b.second;});
                for (int i = 0; i < n_best - best.size(); ++i) {
                    best.emplace(with_dist.at(i).first);
                }
            }
            ++pfi;
        }
        return best;
    }

    void NonDominatedSort(std::vector<T>& R)
    {
        std::vector<const T*> F;
        std::unordered_map<const T*, std::vector<const T*>> S_;
        std::ranges::for_each(R, [&S_](auto& x){S_.emplace(&x, {});});
        for (auto & p : R) {
            for (auto & q : R | std::views::filter([&p](auto& x){return &x != &p;})) {
                if (problem->IsParetoDominatedBy(p, q)) {
                    ++p.dominated_by;
                    S_.at(&q).push_back(&p);
                }
            }
            if (p.dominated_by == 0) {
                F.push_back(&p);
            }
        }
        auto pfi = 1;
        while (F.size() != 0) {
            std::vector<const T*> F_next;
            for (auto p : F) {
                p->pareto_rank = pfi;
                for (auto q : S_.at(p)) {
                    --q->dominated_by;
                    if (q->dominated_by == 0) {
                        F_next.push_back(q);
                    }
                }
            }
            ++pfi;
            F = F_next;
        }
    }
};

#endif //HALAL_NSGAIISOLVER_HPP