//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_HILLCLIMB_H
#define HALAL_HILLCLIMB_H
#include <functional>
#include <iostream>

template <typename T, typename D>
T StochasticHillClimb(D eps, std::function<T()> generate, std::function<T(T, D)> random_neighbour,
                      std::function<float(T)> fitness, const std::function<bool()>& stop_condition) {
    auto p = generate();
    while (!stop_condition()) {
        auto q = random_neighbour(p, eps);
        auto p_fit = fitness(p);
        std::cout << p_fit << std::endl;
        if (fitness(q) < p_fit) {
            p = q;
        }
    }
    return p;
}

template <typename T, typename D>
T SteepestAscentHillClimb(D eps, std::function<T()> generate, std::function<T(T, D)> best_neighbour,
                          std::function<float(T)> fitness, const std::function<bool()>& stop_condition) {
    auto p = generate();
    bool stuck = false;
    while (!stuck && !stop_condition()) {
        auto q = best_neighbour(p, eps);
        if (fitness(q) < fitness(p)) {
            p = q;
        }
        else {
            stuck = true;
        }
    }

    return p;
}

#endif //HALAL_HILLCLIMB_H
