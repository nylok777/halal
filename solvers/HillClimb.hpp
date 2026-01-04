//
// Created by david on 9/17/25.
//
#ifndef HALAL_HILLCLIMB_H
#define HALAL_HILLCLIMB_H
#include "../interfaces-and-templates/StopCondition.hpp"

template <class P, Numeric Number, StopConditionFunctor StopCondType, typename T = P::SolutionType>
T StochasticHillClimb(const P* const solvable, Number eps, StopCondType& stop_condition)
{
    T p = solvable->GenerateInstance();
    while (!StopCondition(stop_condition, p.score)) {
        T q = solvable->GenerateNeighbour(p, eps);
        if (q.score < p.score) {
            p = q;
        }
    }
    return p;
}

#endif //HALAL_HILLCLIMB_H