//
// Created by david on 9/17/25.
//
#ifndef HALAL_HILLCLIMB_H
#define HALAL_HILLCLIMB_H
#include <algorithm>
#include "../interfaces-and-templates/OptimizationProblem.h"
#include "../interfaces-and-templates/StopCondition.hpp"

template <class P, Numeric Number, StopConditionFunctor StopCondType, typename T = P::SolutionType>
T StochasticHillClimb(const P* const solvable, Number eps, StopCondType& stop_condition)
{
    T p = solvable->GenerateInstance();
    while (StopCondition(stop_condition, p.score)) {
        T q = solvable->GenerateNeighbour(p, eps);
        if (q.score < p.score) {
            p = q;
        }
    }
    return p;
}

template <class P, Numeric Number, StopConditionFunctor StopCondType, typename T = P::SolutionType>
T SteepestAscentHillClimb(const P* const solvable, Number eps, StopCondType& stop_condition, const int n_neighbours = 100)
{
    T p = solvable->GenerateInstance();
    bool stuck = false;
    while (!stuck && StopCondition(stop_condition, p.score)) {
        std::vector<T> neighbours;
        neighbours.reserve(n_neighbours);
        for (int i = 0; i < n_neighbours; ++i) {
            neighbours.emplace_back(solvable->GenerateNeighbour(p, eps));
        }
        std::ranges::sort(neighbours, [](const auto& lhs, const auto& rhs) { return lhs.score < rhs.score; });
        T& q = neighbours.front();
        if (q.score < p.score) {
            p = q;
        } else {
            stuck = true;
        }
    }
    return p;
}

#endif //HALAL_HILLCLIMB_H