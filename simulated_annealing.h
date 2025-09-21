//
// Created by david on 9/17/25.
//

#ifndef HALAL_SIMULATED_ANNEALING_H
#define HALAL_SIMULATED_ANNEALING_H

#include <random>
#include <functional>
#include <cmath>
#include <iostream>
template<typename T, typename D>
T SimulatedAnnealing(D eps, const float kB, std::function<T()> generate,
                     std::function<T(T, D)> distance,
                     std::function<float(const T &)> fitness, const std::function<bool()> &stopCondition,
                     const std::function<float(int)> &temperature) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<> real_range;

    auto p = generate();
    auto best_element = p;
    int t = 0;
    while (!stopCondition()) {
        t++;
        const auto q = distance(p, eps);
        const float p_fitness = fitness(p);
        const float q_fitness = fitness(q);
        std::cout << "p: " << p_fitness << std::endl;
        std::cout << "q: " << q_fitness << std::endl;
        if (const auto delta_e = q_fitness - p_fitness; delta_e <= 0) {
            p = q;
            if (q_fitness < fitness(best_element)) {
                best_element = q;
            }
        } else {
            const auto r = delta_e / kB * temperature(t);
            if (const auto x = exp(r); real_range(gen) < x) {
                p = q;
            }
        }
    }
    return best_element;
}
#endif //HALAL_SIMULATED_ANNEALING_H
