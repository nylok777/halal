//
// Created by david on 11/13/25.
//

#ifndef HALAL_SOLUTION_H
#define HALAL_SOLUTION_H
#include "../utils/ConceptsExt.h"
#include <ranges>

// template<typename T, typename = void>
// struct has_numeric_field : std::false_type {};
//
// template<typename T>
// struct has_numeric_field<T, std::void_t<typename T::NumberType>> : std::true_type {};

template<typename T, typename = void>
struct has_objective_func : std::false_type {};

template<typename T>
struct has_objective_func<T, std::void_t<typename T::NumberType>> : std::true_type {};

template<typename R, Numeric N>
struct SolutionCandidate
{
    using RepresentationType = R;
    using NumberType = N;

    R genotype;
    N score;
};

template <typename S, typename R = S::RepresentationType, typename N = S::NumberType>
concept Solution = std::is_base_of_v<SolutionCandidate<R, N>, S>;

template <typename T>
concept ParetoSolution = has_objective_func<T>::value && requires(T t)
{
    { t.pareto_rank } -> Numeric;
    { t.dominated_by } -> Numeric;
};

#endif //HALAL_SOLUTION_H