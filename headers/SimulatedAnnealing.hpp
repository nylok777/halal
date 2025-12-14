//
// Created by david on 9/21/25.
//
#ifndef HALAL_SIMULATEDANNEALINGSOLVER_H
#define HALAL_SIMULATEDANNEALINGSOLVER_H
#include <random>
#include <memory>
#include "StopCondition.h"
#include "Solution.h"

template<typename T, typename = void>
struct is_temperature_func : std::false_type {};

template<typename T>
struct is_temperature_func<T, std::void_t<typename T::temperature_func_tag>> : std::true_type {};

template<typename T>
concept TemperatureFunc = is_temperature_func<T>::value;

struct temperature_func_tag {};

template<Numeric N>
class BoltzmannScheduleTemperature
{
public:
    using temperature_func_tag = ::temperature_func_tag;
    explicit BoltzmannScheduleTemperature(const N temp0) : temp0(temp0) {}
    N operator()(const int t) const
    {
        return temp0 / std::log(t + 1);
    }
private:
    N temp0;
};

template <class P, TemperatureFunc F, StopConditionFunctor S, typename T = P::SolutionType, typename N = T::NumberType>
T SimulatedAnnealing(
    const P* const solvable,
    S& stop_condition,
    F& temperature_func,
    const N eps,
    const N kB)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<> real_range;

    auto p = solvable->GenerateInstance();
    p.score = std::numeric_limits<typename T::NumberType>::max();
    auto best_element = p;
    int t = 0;
    while (StopCondition(stop_condition, p.score)) {
        t++;
        const auto q = solvable->GenerateNeighbour(p, eps);

        if (const auto delta_e = q.score - p.score; delta_e <= 0) {
            p = q;
            if (q.score < best_element.score) {
                best_element = q;
            }
        } else {
            const auto r = delta_e / kB * temperature_func(t);
            if (const auto x = exp(r); real_range(gen) < x) {
                p = q;
            }
        }
    }
    return best_element;
}
#endif //HALAL_SIMULATEDANNEALINGSOLVER_H