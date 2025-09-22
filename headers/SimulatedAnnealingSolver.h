//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_SIMULATEDANNEALINGSOLVER_H
#define HALAL_SIMULATEDANNEALINGSOLVER_H
#include <random>
#include <iostream>
#include "StochasticSolvable.h"

template <typename T>
class SimulatedAnnealingSolver
{
    StochasticSolvable<T>* problem;
    float temp0;

public:
    SimulatedAnnealingSolver(StochasticSolvable<T>& problem, const float& temp0);
    [[nodiscard]] float BoltzmannScheduleTemperature(const int) const;
    std::pair<T, float> SimulatedAnnealing(const float& eps, const float& kB);
};

template <typename T>
SimulatedAnnealingSolver<T>::SimulatedAnnealingSolver(StochasticSolvable<T>& problem, const float& temp0)
    : problem(&problem), temp0(temp0) {
}

template <typename T>
float SimulatedAnnealingSolver<T>::BoltzmannScheduleTemperature(const int t) const {
    return temp0 / std::log(t + 1);
}

template <typename T>
std::pair<T, float> SimulatedAnnealingSolver<T>::SimulatedAnnealing(const float& eps, const float& kB) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<> real_range;
    auto p = problem->GenerateElement();
    auto best_element = p;
    int t = 0;
    while (!problem->StopCondition()) {
        t++;
        const auto q = problem->GenerateNeighbour(p, eps);
        const float p_fitness = problem->Objective(p);
        const float q_fitness = problem->Objective(q);
        std::cout << "p: " << p_fitness << std::endl;
        if (const auto delta_e = q_fitness - p_fitness; delta_e <= 0) {
            p = q;
            if (q_fitness < problem->Objective(best_element)) {
                best_element = q;
            }
        }
        else {
            const auto r = delta_e / kB * BoltzmannScheduleTemperature(t);
            if (const auto x = exp(r); real_range(gen) < x) {
                p = q;
            }
        }
    }
    auto best_fitness = problem->Objective(best_element);
    return std::pair(best_element, best_fitness);
}
#endif //HALAL_SIMULATEDANNEALINGSOLVER_H