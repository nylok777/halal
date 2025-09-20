//
// Created by david on 9/17/25.
//
#include <vector>
#include <functional>
#include <random>
#include <set>
#include <cmath>
#include "simulated_annealing.h"
using std::set;
using std::vector;
using std::function;

template<typename T>
T SimulatedAnnealing(const vector<T> domain, const float &eps, const float &kB,
                     function<T(T, const vector<T> &, float)> distance,
                     function<float(const T &)> fitness, const function<bool()> &stopCondition,
                     const function<float(const int &)> &temperature) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> int_range(0, domain.size() - 1);
    std::uniform_real_distribution<> real_range;

    const auto p = domain[int_range(gen)];
    const auto best_element = p;
    int t = 0;
    while (!stopCondition()) {
        t++;
        const auto q = distance(p, domain, eps);
        const float p_fitness = fitness(p);
        float q_fitness = fitness(q);
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
