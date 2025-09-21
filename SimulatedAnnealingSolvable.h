//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_SIMULATEDANNEALINGSOLVABLE_H
#define HALAL_SIMULATEDANNEALINGSOLVABLE_H
#include <iostream>
#include <random>
#include "Solvable.h"

template<typename Numeric, typename T, typename D>
class SimulatedAnnealingSolvable : public Solvable<T, D>{

protected:
    ~SimulatedAnnealingSolvable() = default;

    Numeric temp0;
public:
    virtual float BoltzmannScheduleTemperature(int) = 0;

    std::pair<T, float> SimulatedAnnealing(D eps, const float kB) {
        std::random_device rnd;
        std::mt19937 gen{rnd()};
        std::uniform_real_distribution<> real_range;
        auto p = this->GenerateElement();
        auto best_element = p;
        int t = 0;
        while (!this->StopSearch()) {
            t++;
            const auto q = this->GenerateNeighbour(p, eps);
            const float p_fitness = this->Objective(p);
            const float q_fitness = this->Objective(q);
            std::cout << "p: " << p_fitness << std::endl;
            if (const auto delta_e = q_fitness - p_fitness; delta_e <= 0) {
                p = q;
                if (q_fitness < this->Objective(best_element)) {
                    best_element = q;
                }
            } else {
                const auto r = delta_e / kB * this->BoltzmannScheduleTemperature(t);
                if (const auto x = exp(r); real_range(gen) < x) {
                    p = q;
                }
            }
        }
        auto best_fitness = this->Objective(best_element);
        return std::pair(best_element, best_fitness);
    }
};

#endif //HALAL_SIMULATEDANNEALINGSOLVABLE_H