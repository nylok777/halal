//
// Created by david on 9/21/25.
//
#include <random>
#include <algorithm>
#include "GeneticSolver.h"

template <typename T>
GeneticSolver<T>::GeneticSolver(GeneticSolvable<T> problem) : problem(problem){}

template<typename T>
std::vector<T> GeneticSolver<T>::InitPopulation(const int& size)
{
    auto population = std::vector<std::vector<int>>();
    for (int i = 0; i < size; ++i)
    {
        auto p = problem.GenerateElement();
        population.push_back(p);
    }
    return population;
}

template <typename T>
std::vector<float> GeneticSolver<T>::Evaluation(const std::vector<T>& population)
{
    auto fitness = std::vector<float>();
    for (auto p : population)
    {
        fitness.push_back(problem.Objective(p));
    }
    return fitness;
}

template <typename T>
std::vector<T> GeneticSolver<T>::Selection(const std::vector<T>& population, const std::vector<float>& fitness, const int& m)
{
    auto p_fit_pairs = std::vector<std::pair<std::vector<int>, float>>();
    for (int i = 0; i < population.size(); ++i)
    {
        p_fit_pairs.emplace_back(population[i], fitness[i]);
    }
    std::sort(p_fit_pairs.begin(), p_fit_pairs.end());
    auto selected = std::vector<std::vector<int>>();
    for (int i = m - 1; i >= 0; --i)
    {
        selected.push_back(p_fit_pairs[i].first);
    }
    return selected;
}

template<typename T>
std::pair<T, float> GeneticSolver<T>::GeneticAlgorithm(const int& k, const int& size, const int& m)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, m - 1);

    auto population = InitPopulation(size);
    auto pop_fitness = Evaluation(population);
    T p_best;
    int best_idx = 0;
    for (int i = 1; i < population.size(); ++i)
    {
        if (pop_fitness[i] < pop_fitness[best_idx])
        {
            best_idx = i;
            p_best = population[i];
        }
    }
    while (!problem.StopSearch())
    {
        auto parents = Selection(pop_fitness, m);
        auto new_gen = std::vector<T>();
        while (new_gen.size() < parents.size())
        {
            auto c_parents = std::vector<T>();
            for (int i = 0; i < k; ++i)
            {
                c_parents.push_back(parents[dist(gen)]);
            }
            auto c = problem.CrossOver(c_parents);
            c = problem.Mutate(c);
            new_gen.push_back(c);
        }
        population = Reinsertion(population, new_gen);
        pop_fitness = Evaluation(population);
        best_idx = 0;
        for (int i = 0; i < population.size(); ++i)
        {
            if (pop_fitness[i] < pop_fitness[best_idx])
            {
                best_idx = i;
                p_best = population[i];
            }
        }
    }
    return p_best;
}
