//
// Created by david on 9/21/25.
//
#ifndef HALAL_GENETICSOLVER_H
#define HALAL_GENETICSOLVER_H
#include <vector>
#include <random>
#include <algorithm>

#include "../interfaces-and-templates/OptimizationProblem.h"
#include "../interfaces-and-templates/StopCondition.hpp"

template<std::ranges::input_range R>
requires std::ranges::forward_range<R> && Solution<std::ranges::range_value_t<R>>
auto Selection(R&& population, const int n_parents) -> std::ranges::view auto
{
    std::ranges::sort(population, [](const auto& a, const auto& b) -> bool { return a.score < b.score; });
    return std::ranges::views::take(population, n_parents);
}

template<std::ranges::input_range R1, std::ranges::input_range R2, class Gen>
requires std::ranges::forward_range<R1> && std::ranges::forward_range<R2> && std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
void Reinsertion(R1&& population, R2&& new_gen, Gen&& gen)
{
    std::ranges::shuffle(population, gen);
    std::ranges::copy(new_gen, std::ranges::begin(population));
}

template<class P, StopConditionFunctor S, typename T = P::SolutionType>
requires Solution<T>
auto GeneticAlgorithm(
    const P* const solvable,
    S& stop_condition,
    const int n_parents,
    const int population_size,
    const int parents_pool_size,
    const int new_gen_size,
    const float mutation_rate) -> T
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dist{0, parents_pool_size - 1};
    std::uniform_real_distribution<float> real_dist{0.0, 1.0};

    std::vector<T> population;
    population.reserve(population_size);
    for (int i = 0; i < population_size; ++i) {
        population.push_back(solvable->GenerateInstance());
    }
    auto p_best = *std::ranges::min_element(population, [](const auto& a, const auto& b) -> bool
    {
        return a.score < b.score;
    });
    while (StopCondition(stop_condition, p_best.score)) {
        auto parents = Selection(population, parents_pool_size);
        std::vector <T> new_gen;
        new_gen.reserve(new_gen_size);
        for (int i = 0; i < new_gen_size; ++i) {
            std::vector <T> current_child_parents;
            current_child_parents.reserve(n_parents);
            for (int j = 0; j < n_parents; ++j) {
                current_child_parents.push_back(parents[dist(gen)]);
            }
            auto child = solvable->CrossOver(current_child_parents);
            if (real_dist(gen) < mutation_rate) {
                solvable->Mutate(child);
            }
            new_gen.push_back(child);
        }
        Reinsertion(population, new_gen, gen);
        auto q_best = *std::ranges::min_element(population, [](const auto& a, const auto& b) -> bool
        {
            return a.score < b.score;
        });
        if (q_best < p_best) p_best = q_best;
    }
    return p_best;
}
#endif //HALAL_GENETICSOLVER_H