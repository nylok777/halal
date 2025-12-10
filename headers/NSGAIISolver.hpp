//
// Created by david on 12/10/25.
//

#ifndef HALAL_NSGAIISOLVER_HPP
#define HALAL_NSGAIISOLVER_HPP
#include <algorithm>
#include <memory>
#include <set>
#include <unordered_map>

#include "DynamicMatrix.hpp"
#include "ProblemRepresentation.h"
#include "SolutionCandidate.h"
#include "StopCondition.h"

template<HasParetoRank T, StopConditionFunctor S, typename P, Numeric N>
requires std::derived_from<P, ParetoOptimizationProblem<T, N>>
class NSGAIISolver
{
public:
    using crowding_distance = std::pair<T, N>;
    explicit NSGAIISolver(P&& problem) : problem(std::make_unique<P>(problem)) {}
    std::vector<T> operator()(S& stop_condition);
private:
    std::unique_ptr<P> problem;
    void CrowdingDistance(std::vector<T>& F)
    {
        std::vector<crowding_distance> D; D.reserve(F.size());
        std::ranges::views::transform(F, std::back_inserter(D),[](auto& x){ return crowding_distance{x, 0}; });
        for (auto& func : problem->GetObjectives()) {
            std::vector<N> func_results; func_results.reserve(D.size());
            std::ranges::transform(D, std::back_inserter(func_results), [&func](auto& p){return func(p.first);});
            std::ranges::sort(D, [&func](auto& p, auto& q){return func(p.first) < func(q.first);});
            D.at(0).second, D.back().second = std::numeric_limits<N>::max();
            for (int i = 1; i < D.size() - 1; ++i) {
                D.at(i).second += (func(D.at(i+1).first) - func(D.at(i-1).first));
            }
        }
    }
    /*
    void CrowdingDistance(std::vector<T>& F)
    {
        DynamicMatrix<N> G{F.size()};
        for (int i = 0; i < F.size(); ++i) {
            for (auto score : problem->Objectives(F.at(i))) {
                G.PushBack(i,score);
            }
        }
        for (int i = 0; i < G.ColumnCount(); ++i) {
            for (auto & obj_result : G.Column(i)) {

                std::ranges::sort()
            }
        }

    }
    */
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