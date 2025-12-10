//
// Created by david on 9/21/25.
//
#ifndef HALAL_PROBLEM_H
#define HALAL_PROBLEM_H
#include <functional>

#include "SolutionCandidate.h"

#include <vector>

template <typename T, typename Number>
class ProblemRepresentation
{
public:
    using NumberType = Number;
    virtual ~ProblemRepresentation() = default;
    virtual T GenerateInstance() const = 0;
    virtual Number Objective(const T&) const = 0;
};

template<HasParetoRank T, Numeric N>
class ParetoOptimizationProblem
{
public:
    virtual ~ParetoOptimizationProblem() = default;
    virtual T GenerateInstance() const = 0;
    virtual bool IsParetoDominatedBy(const T& a, const T& b) const = 0;
    virtual std::vector<std::function<N(T&)>> GetObjectives() const = 0;
    virtual std::vector<N> Objectives(T&) const = 0;
};

#endif //HALAL_PROBLEM_H