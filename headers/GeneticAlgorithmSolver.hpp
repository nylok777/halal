//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_GENETICSOLVER_H
#define HALAL_GENETICSOLVER_H
#include <vector>
#include <random>
#include <algorithm>
#include <memory>
#include "GeneticSolvable.h"
#include "StopCondition.h"


template <SolutionCandidate T, StopConditionFunctor S>
class GeneticAlgorithmSolver
{
public:
    GeneticAlgorithmSolver(std::unique_ptr<GeneticSolvable<T>>&& solvable, S stop_condition)
        : solvable(std::move(solvable)), stop_condition(stop_condition) {}

private:
    std::vector<T> InitPopulation(const int population_size)
    {
        std::vector < T > population;
        population.reserve(population_size);
        for (int i = 0; i < population_size; ++i) {
            population.push_back(solvable->GenerateInstance());
        }
        return population;
    }

    std::vector<T> Selection(std::vector<T>& population, const int parents_pool_size)
    {
        std::ranges::sort(population, [](const auto& x, const auto& y) { return x.score < y.score; });
        std::vector < T > selected;
        selected.reserve(parents_pool_size);
        for (int i = 0; i < parents_pool_size; ++i) {
            selected.push_back(population[i]);
        }
        return selected;
    }

    std::vector<T> Reinsertion(const std::vector<T>& old_gen, const std::vector<T>& new_gen)
    {
        std::vector < T > new_population;
        new_population.reserve(old_gen.size());
        for (const auto& new_gen_instance : new_gen) new_population.push_back(new_gen_instance);
        for (int i = 0; i < old_gen.size() - new_gen.size(); ++i) {
            new_population.push_back(old_gen[i]);
        }
        return new_population;
    }
public:
    T GeneticAlgorithm(
        const int n_parents,
        const int population_size,
        const int parents_pool_size,
        const int new_gen_size,
        const float mutation_rate);

private:
    std::unique_ptr<GeneticSolvable<T>> solvable;
    S stop_condition;
};


template <SolutionCandidate T, StopConditionFunctor S>
T GeneticAlgorithmSolver<T, S>::GeneticAlgorithm(
    const int n_parents,
    const int population_size,
    const int parents_pool_size,
    const int new_gen_size,
    const float mutation_rate)
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dist{0, parents_pool_size - 1};
    std::uniform_real_distribution<float> real_dist{0.0, 1.0};

    auto population = InitPopulation(population_size);
    auto p_best = solvable->GetBest(population);
    while (StopCondition(stop_condition, p_best.score)) {
        auto parents = Selection(population, parents_pool_size);
        std::vector < T > new_gen;
        new_gen.reserve(new_gen_size);
        for (int i = 0; i < new_gen_size; ++i) {
            std::vector < T > current_child_parents;
            current_child_parents.reserve(n_parents);
            for (int j = 0; j < n_parents; ++j) {
                current_child_parents.push_back(parents[dist(gen)]);
            }
            auto child = solvable->CrossOver(current_child_parents);
            if (real_dist(gen) < mutation_rate) solvable->Mutate(child);
            new_gen.push_back(child);
        }
        population = Reinsertion(population, new_gen);
        auto q_best = solvable->GetBest(population);
        if (q_best < p_best) p_best = q_best;
    }
    return p_best;
}
#endif //HALAL_GENETICSOLVER_H