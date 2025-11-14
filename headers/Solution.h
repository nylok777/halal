//
// Created by david on 11/13/25.
//

#ifndef HALAL_SOLUTION_H
#define HALAL_SOLUTION_H
#include <concepts>

template <typename T>
concept Numeric = std::integral<std::remove_cvref_t<T>> ||
    std::floating_point<std::remove_cvref_t<T>>;

template <typename C>
concept Solution = requires(C sol)
{
    { sol.rep };
    { sol.score } -> Numeric;
};

#endif //HALAL_SOLUTION_H