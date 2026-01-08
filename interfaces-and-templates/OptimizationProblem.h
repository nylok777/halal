//
// Created by david on 9/21/25.
//
#ifndef HALAL_PROBLEM_H
#define HALAL_PROBLEM_H
#include <functional>

#include "Solution.h"

#include <vector>

template <Solution T>
class OptimizationProblem
{
public:
    using NumberType = T::NumberType;
    using SolutionType = T;
    using Genotype = T::RepresentationType;

    virtual ~OptimizationProblem() = default;
    virtual auto GenerateSolution() const -> T = 0;
    virtual auto Objective(const Genotype&) const -> NumberType = 0;
};

template<typename T>
concept SingleObjectiveProblem = std::is_base_of_v<OptimizationProblem<typename T::SolutionType>, T>;

template <Solution T>
class IRandomNeighbour
{
public:
    virtual ~IRandomNeighbour() = default;
    virtual auto GenerateNeighbour(const T&, float eps) const -> T = 0;
};

template<Solution T>
class IDirectNeighbour
{
public:
    virtual ~IDirectNeighbour() = default;
    virtual auto GetDirectNeighbour(const T& elem, float eps) const -> T = 0;
};

template<ParetoSolution T, Numeric N>
class ParetoOptimizationProblem
{
public:
    using NumberType = N;
    using SolutionType = T;
    virtual ~ParetoOptimizationProblem() = default;
    virtual auto GenerateSolution() const -> T = 0;
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