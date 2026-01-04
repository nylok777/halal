//
// Created by david on 1/2/26.
//

#ifndef HALAL_RANDOMSEARCH_HPP
#define HALAL_RANDOMSEARCH_HPP
#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>
#include <random>

#include "interfaces-and-templates/IDimensions.h"
#include "utils/ConceptsExt.h"
#include "interfaces-and-templates/OptimizationProblem.h"
#include "interfaces-and-templates/StopCondition.hpp"

constexpr std::vector<float> CalculateVectorDirection(const std::vector<float>& vec)
{
    const auto squared = std::accumulate(vec.begin(), vec.end(), 0.0f, [](auto x, auto y)
    {
        return std::pow(x, 2) + std::pow(y, 2);
    });
    auto magnitude = std::sqrt(squared);
    std::vector<float> direction;
    direction.reserve(vec.size());
    std::ranges::transform(vec, std::back_inserter(direction), [magnitude](const auto elem)
    {
        return elem / magnitude;
    });
    return direction;
}

template<class P, StopConditionFunctor S, Numeric N, typename T = P::SolutionType>
requires Solution<T>
T RandomSearch(
    const P* const solvable,
    S&& stop_condition,
    const std::vector<std::function<N(const typename T::RepresentationType&)>> constraints,
    const std::vector<float>& mean,
    const std::vector<float>& std_deviance)
{
    std::mt19937 gen{std::random_device{}()};
    std::vector<float> steps(mean.size());
    auto p = solvable->GenerateInstance();
    while (!StopCondition(std::forward<S>(stop_condition), p.score)) {
        for (size_t i = 0; i < steps.size(); ++i) {
            std::normal_distribution dist{mean.at(i), std_deviance.at(i)};
            steps.at(i) = dist(gen);
        }
        auto direction = CalculateVectorDirection(steps);
        const auto q_genotype = p + direction;
        if (std::ranges::all_of(constraints, [&q_genotype](const auto& c) { return c(q_genotype) >= 0; })) {
            auto score = solvable->Objective(q_genotype);
            if (score < p.score) {
                p = T{q_genotype, score};
            }
        }
    }
    return p;
}

#endif //HALAL_RANDOMSEARCH_HPP