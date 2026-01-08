//
// Created by david on 9/21/25.
//
#ifndef HALAL_GENETICSOLVER_H
#define HALAL_GENETICSOLVER_H
#include <utility>
#include <vector>
#include <random>
#include <algorithm>

#include "OptimizationProblem.h"
#include "StopCondition.hpp"
#include "ranges_ext.hpp"

template<std::ranges::forward_range R, std::integral Int> requires Solution<std::ranges::range_value_t<R>>
auto Selection(R& population, const Int n_parents) -> std::ranges::view auto
{
    std::ranges::sort(population, [](const auto& a, const auto& b) -> bool { return a.score < b.score; });
    return std::ranges::views::take(population, n_parents);
}

template<std::ranges::forward_range R, std::integral Int, class Gen>
requires Solution<std::ranges::range_value_t<R>> && std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
auto OrderedSelection(R& population, const Int n_parents, Gen& gen, const Int k) -> std::ranges::view auto
{
    std::ranges::sort(population, [](const auto& a, const auto& b) -> bool { return a.score < b.score; });
    const auto gamma = 1 / (1 - (std::log(k) / std::log(n_parents)));
    const std::uniform_real_distribution dist;
    std::vector<size_t> indices;
    indices.reserve(n_parents);
    for (int i = 0; i < n_parents; ++i) {
        indices.push_back(static_cast<size_t>(std::pow(dist(gen), gamma) * std::ranges::size(population)));
    }
    return take_at(population, std::move(indices));
}

template<std::ranges::forward_range R, std::integral Int, class Gen> requires Solution<std::ranges::range_value_t<R>>
void TournamentSelection(R& population, const Int n_parents, Gen& gen, const Int k)
{
    std::vector<size_t> winners;
    winners.reserve(n_parents);
    auto start = std::ranges::begin(population);
    std::vector<int> win_counts(k);
    while (winners.size() < n_parents) {
        auto contestants = sample_view(population, k, gen);
        for (size_t i = 0; std::cmp_less(i , k); ++i) {
            for (size_t j = 0; std::cmp_less(j , k); ++j) {
                if (i == j) continue;
                if (contestants[i]->score < contestants[j]->score) ++win_counts.at(i); else ++win_counts.at(j);
            }
        }
        auto max = std::distance(win_counts.begin(), std::ranges::max_element(win_counts));
        auto loc = std::distance(start, (contestants.begin() + max));
        winners.push_back(static_cast<size_t>(loc));
        std::ranges::for_each(win_counts, [](auto& x) { x = 0; });
    }
}

template<std::ranges::forward_range R1, std::ranges::forward_range R2, class Gen>
requires std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
void Reinsertion(R1& population, R2& descendants, Gen& gen)
{
    std::ranges::shuffle(population, gen);
    std::ranges::copy(descendants, std::ranges::begin(population));
}

template<std::ranges::forward_range R1, std::ranges::forward_range R2, Solution T = std::ranges::range_value_t<R1>>
void ElitistReinsertion(R1& population, R2& descendants)
{
    if (!std::ranges::is_sorted(population)) {
        std::ranges::sort(population);
    }
    std::ranges::sort(descendants);
    auto pop_size = std::ranges::size(population);
    std::vector<T> temp;
    temp.reserve(pop_size + std::ranges::size(descendants));
    std::ranges::merge(population, descendants, std::back_inserter(temp));
    temp.resize(pop_size);
    move_n(temp, std::ranges::begin(population));
}

template<class P, StopConditionFunctor S, std::integral Int, std::floating_point Float, Solution T = P::SolutionType>
T GeneticAlgorithm(
    const P* solvable,
    S& stop_condition,
    const Int population_size,
    const Int n_parents,
    const Int parents_pool_size,
    const Int descendants_size,
    const Float mutation_rate)
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<Int> int_dist{0, parents_pool_size - 1};
    std::uniform_real_distribution<Float> real_dist{0.0, 1.0};

    std::vector<T> population;
    population.reserve(population_size);
    for (int i = 0; i < population_size; ++i) {
        population.push_back(solvable->GenerateSolution());
    }
    auto p_best = *std::ranges::min_element(population, [](const auto& a, const auto& b) -> bool
    {
        return a.score < b.score;
    });
    while (!StopCondition(stop_condition, p_best.score)) {
        auto mating_pool = Selection(population, parents_pool_size);
        std::vector <T> descendants;
        descendants.reserve(descendants_size);
        for (int i = 0; i < descendants_size; ++i) {
            std::vector <T> parents;
            parents.reserve(n_parents);
            for (int j = 0; j < n_parents; ++j) {
                parents.push_back(mating_pool[int_dist(gen)]);
            }
            auto child = solvable->CrossOver(parents);
            if (real_dist(gen) < mutation_rate) {
                solvable->Mutate(child);
            }
            descendants.push_back(child);
        }
        Reinsertion(population, descendants, gen);
        auto q_best = *std::ranges::min_element(population, [](const auto& a, const auto& b) -> bool
        {
            return a.score < b.score;
        });
        if (q_best < p_best) p_best = q_best;
    }
    return p_best;
}

template<class P, StopConditionFunctor S, std::integral Int, std::floating_point Float, Solution T = P::SolutionType>
T GeneticAlgorithm(const P* solvable, S& stop_condition, const Int population_size = Int{100}, const Float mutation_rate = 0.2f)
{
    return GeneticAlgorithm(
        solvable,
        stop_condition,
        population_size,
        Int{2},
        static_cast<Int>(population_size * 0.2f),
        population_size,
        mutation_rate);
}
#endif //HALAL_GENETICSOLVER_H