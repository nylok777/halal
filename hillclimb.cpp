#include "hillclimb.h"
#include <vector>
#include <random>
#include <functional>
#include <iostream>
using std::vector;
using std::function;

template<typename T>
T SteepestAscentHillClimb(vector<T> domain, float epsilon, function<vector<T>(T, vector<T>, float)> distance,
                          function<float(T)> fitness, const function<bool()> &stopCondition) {
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_int_distribution<> range(0, domain.size() - 1);

    auto p = domain[range(gen)];
    bool stuck = false;
    while (!stuck && !stopCondition()) {
        int best = 0;
        auto neighbours = distance(p, domain, epsilon);
        float best_fitness = fitness(neighbours[best]);

        for (size_t i = 0; i < neighbours.size(); i++) {
            if (const float current_fitness = fitness(neighbours[i]); current_fitness < best_fitness) {
                best = i;
                best_fitness = current_fitness;
            }
        }

        if (best_fitness < fitness(p)) {
            p = neighbours[best];
        } else {
            stuck = true;
        }
    }

    return p;
}
