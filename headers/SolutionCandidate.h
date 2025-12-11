//
// Created by david on 11/13/25.
//

#ifndef HALAL_SOLUTION_H
#define HALAL_SOLUTION_H
#include <concepts>
#include <ranges>

template <typename T>
concept Numeric = std::integral<std::remove_cvref_t<T>> ||
    std::floating_point<std::remove_cvref_t<T>>;

template <typename T>
concept SolutionCandidate = requires(T sol)
{
    { sol.rep };
    { sol.score } -> Numeric;
};

template <typename T>
concept HasParetoRank = requires(T t)
{
    { t.pareto_rank } -> Numeric;
    { t.dominated_by } -> Numeric;
};

#endif //HALAL_SOLUTION_H