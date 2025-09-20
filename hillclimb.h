//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_HILLCLIMB_H
#define HALAL_HILLCLIMB_H
#include <functional>
#include <iostream>

template<typename T, typename D>
T StochasticHillClimb(D epsilon, std::function<T()> generate, std::function<T(T, float)> distance,
                      std::function<float(T)> fitness, const std::function<bool()>& stopCondition) {
    auto p = generate();
    while (!stopCondition()) {
        auto q = distance(p, epsilon);
        auto p_fit = fitness(p);
        std::cout << p_fit << std::endl;
        if (fitness(q) < p_fit) {
            p = q;
        }
    }

    return p;
}
#endif //HALAL_HILLCLIMB_H