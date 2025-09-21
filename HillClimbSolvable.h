//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_HILLCLIMB_H
#define HALAL_HILLCLIMB_H
#include <iostream>
#include <random>

#include "Solvable.h"

template<typename T, typename D>
class HillClimbSolvable : Solvable<T, D>{
protected:
    ~HillClimbSolvable() = default;

public:

    virtual T StochasticHillClimb(D eps) {
        auto p = this->GenerateElement();
        while (!this->StopSearch()) {
            auto q = this->GenerateNeighbour(p, eps);
            auto p_fit = this->Objective(p);
            std::cout << p_fit << std::endl;
            if (this->Objective(q) < p_fit) {
                p = q;
            }
        }
        return p;
    }

    virtual T SteepestAscentHillClimb(D eps) {
        auto p = this->GenerateElement();
        bool stuck = false;
        while (!stuck && !this->StopSearch()) {
            int best = 0;
            auto neighbours = this->AllNeighbours(p, eps);
            float best_neighbour_fitness = this->Objective(neighbours[best]);

            for (size_t i = 0; i < neighbours.size(); i++) {
                if (const float neighbour_fitness = this->Objective(neighbours[i]); neighbour_fitness < best_neighbour_fitness) {
                    best = i;
                    best_neighbour_fitness = neighbour_fitness;
                }
            }

            if (best_neighbour_fitness < this->Objective(p)) {
                p = neighbours[best];
            } else {
                stuck = true;
            }
        }

        return p;
    }

};
#endif //HALAL_HILLCLIMB_H