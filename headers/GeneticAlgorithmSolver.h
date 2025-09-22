//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_GENETICSOLVER_H
#define HALAL_GENETICSOLVER_H
#include <vector>
#include <random>
#include <algorithm>
#include "GeneticSolvable.h"

template<typename T>
class GeneticAlgorithmSolver
{
    GeneticSolvable<T>* problem;

public:
    explicit GeneticAlgorithmSolver(GeneticSolvable<T>& problem);
    std::vector<T> InitPopulation(const int&);
    std::vector<float> Evaluation(const std::vector<T>&);
    std::vector<T> Selection(const std::vector<T>&, const std::vector<float>&, const int&);
    std::vector<T> Reinsertion(std::vector<T>&, std::vector<T>&);

    std::pair<T, float> GeneticAlgorithm(const int& k, const int& population_size, const int& parents_size);
};

template <typename T>
GeneticAlgorithmSolver<T>::GeneticAlgorithmSolver(GeneticSolvable<T>& problem) : problem(&problem) {}

template <typename T>
std::vector<T> GeneticAlgorithmSolver<T>::InitPopulation(const int& size) {
    auto population = std::vector<std::vector<int>>();
    for (int i = 0; i < size; ++i) {
        auto p = problem->GenerateElement();
        population.push_back(p);
    }
    return population;
}

template <typename T>
std::vector<float> GeneticAlgorithmSolver<T>::Evaluation(const std::vector<T>& population) {
    auto fitness = std::vector<float>();
    for (auto p : population) {
        fitness.push_back(problem->Objective(p));
    }
    return fitness;
}

template <typename T>
std::vector<T> GeneticAlgorithmSolver<T>::Selection(const std::vector<T>& population, const std::vector<float>& fitness,
                                           const int& m) {
    auto p_fit_pairs = std::vector<std::pair<T, float>>();
    for (int i = 0; i < population.size(); ++i) {
        p_fit_pairs.emplace_back(population[i], fitness[i]);
    }
    std::sort(p_fit_pairs.begin(), p_fit_pairs.end(), [](std::pair<T, float> a, std::pair<T, float> b) {
        return a.second < b.second;
    });
    auto selected = std::vector<std::vector<int>>();
    for (int i = 0; i < m; ++i) {
        selected.push_back(p_fit_pairs[i].first);
    }
    return selected;
}

template <typename T>
std::vector<T> GeneticAlgorithmSolver<T>::Reinsertion(std::vector<T>& old_gen, std::vector<T>& new_gen) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, 1);
    auto new_population = std::vector<T>();
    auto new_count = 0;
    auto old_count = 0;
    for (int i = 0; i < old_gen.size(); ++i) {
        int choice = dist(gen);
        if (choice == 1 && new_count < new_gen.size()) {
            new_population.push_back(new_gen[new_count]);
            new_count++;
        }
        else if (old_count < old_gen.size()) {
            new_population.push_back(old_gen[old_count]);
            old_count++;
        }
    }
    return new_population;
}


template <typename T>
std::pair<T, float> GeneticAlgorithmSolver<T>::GeneticAlgorithm(const int& k, const int& population_size, const int& parents_size) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, parents_size - 1);

    auto population = InitPopulation(population_size);
    auto pop_fitness = Evaluation(population);
    auto p_best = problem->GetBest(population, pop_fitness);
    while (!problem->StopCondition()) {
        auto parents = Selection(population, pop_fitness, parents_size);
        auto new_gen = std::vector<T>();
        while (new_gen.size() < parents.size()) {
            auto c_parents = std::vector<T>();
            for (int i = 0; i < k; ++i) {
                c_parents.push_back(parents[dist(gen)]);
            }
            auto c = problem->CrossOver(c_parents);
            c = problem->Mutate(c);
            new_gen.push_back(c);
        }
        population = Reinsertion(population, new_gen);
        pop_fitness = Evaluation(population);
        auto q_best = problem->GetBest(population, pop_fitness);
        if (q_best.second < p_best.second) p_best = q_best;
    }
    return p_best;
}
#endif //HALAL_GENETICSOLVER_H