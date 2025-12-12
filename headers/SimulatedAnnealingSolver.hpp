//
// Created by david on 9/21/25.
//
#ifndef HALAL_SIMULATEDANNEALINGSOLVER_H
#define HALAL_SIMULATEDANNEALINGSOLVER_H
#include <random>
#include <memory>
#include "StopCondition.h"
#include "Solution.h"
#include "StochasticSolvable.h"

template <Solution T, StopConditionFunctor S>
class SimulatedAnnealingSolver
{
public:
    SimulatedAnnealingSolver(
        std::unique_ptr<StochasticSolvable<T>>&& solvable,
        S stop_condition,
        const float temp0)
        :
        solvable(std::move(solvable)),
        stop_condition(stop_condition),
        temp0(temp0) {}

    [[nodiscard]] float BoltzmannScheduleTemperature(const int t) const { return temp0 / std::log(t + 1); }

    T SimulatedAnnealing(const float eps, const float kB);

private:
    std::unique_ptr<StochasticSolvable<T>> solvable;
    S stop_condition;
    float temp0;
};

template <Solution T, StopConditionFunctor S>
T SimulatedAnnealingSolver<T, S>::SimulatedAnnealing(const float eps, const float kB)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<> real_range;

    auto p = solvable->GenerateInstance();
    auto best_element = p;
    int t = 0;
    float p_fitness = std::numeric_limits<float>::max();
    while (StopCondition(stop_condition, p_fitness)) {
        t++;
        const auto q = solvable->GenerateNeighbour(p, eps);

        if (const auto delta_e = q.score - p.score; delta_e <= 0) {
            p = q;
            if (q.score < best_element.score) {
                best_element = q;
            }
        } else {
            const auto r = delta_e / kB * BoltzmannScheduleTemperature(t);
            if (const auto x = exp(r); real_range(gen) < x) {
                p = q;
            }
        }
    }
    return best_element;
}
#endif //HALAL_SIMULATEDANNEALINGSOLVER_H