//
// Created by david on 9/21/25.
//
#ifndef HALAL_PROBLEM_H
#define HALAL_PROBLEM_H
#include <functional>

#include "Solution.h"

#include <vector>

template <Solution T, Numeric N>
class OptimizationProblem
{
public:
    using NumberType = N;
    using SolutionType = T;
    virtual ~OptimizationProblem() = default;
    virtual auto GenerateInstance() const -> T = 0;
    virtual auto Objective(const T&) const -> N = 0;
};

template<ParetoSolution T, Numeric N>
class ParetoOptimizationProblem
{
public:
    using NumberType = N;
    using SolutionType = T;
    virtual ~ParetoOptimizationProblem() = default;
    virtual auto GenerateInstance() const -> T = 0;
    virtual auto GetObjectives() const -> std::vector<std::function<N(const T&)>> = 0;
};

template<ParetoSolution T>
class IParetoDominanceComparator
{
public:
    virtual ~IParetoDominanceComparator() = default;
    virtual auto operator()(const T& a, const T& b) const -> bool = 0;
};

#endif //HALAL_PROBLEM_H