//
// Created by david on 11/13/25.
//

#ifndef HALAL_SOLUTION_H
#define HALAL_SOLUTION_H
#include "../utils/ConceptsExt.h"
#include <ranges>

template<typename T, typename = void>
struct has_numeric_field : std::false_type {};

template<typename T>
struct has_numeric_field<T, std::void_t<typename T::NumberType>> : std::true_type {};

template<typename T, typename = void>
struct has_objective_func : std::false_type {};

template<typename T>
struct has_objective_func<T, std::void_t<typename T::ObjectiveReturnType>> : std::true_type {};

template <typename T, typename P = T::RepresentationType>
concept Solution = has_numeric_field<T>::value && requires(T sol)
{
    { sol.rep };
    { sol.score } -> Numeric;
};

template <typename T>
concept ParetoSolution = has_objective_func<T>::value && requires(T t)
{
    { t.pareto_rank } -> Numeric;
    { t.dominated_by } -> Numeric;
};

#endif //HALAL_SOLUTION_H