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

template <typename T>
T SimulatedAnnealing(const set<T> domain, const float& eps, const float& kB,
                     function<T*(T, const set<T>&, float)> distance,
                     function<float(const T&)> fitness, const function<bool()>& stopCondition,
                     const function<float(const int&)>& temperature)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> range = new std::uniform_int_distribution(0, domain.size() - 1);
    std::uniform_real_distribution<> realRange;

    const T* element = domain[range(gen)];
    const T* optElement = element;
    int t = 0;
    while (!stopCondition())
    {
        t++;
        const T* neighbour = distance(*element, domain, eps);
        auto elemFitness = fitness(*element);
        auto neighbourFitness = fitness(*neighbour);
        auto deltaE = neighbourFitness - elemFitness;
        if (deltaE <= 0)
        {
            element = neighbour;
            if (neighbourFitness < fitness(*optElement))
            {
                optElement = neighbour;
            }
        }
        else
        {
            auto r = (deltaE / kB * temperature(t));
            auto p = exp(r);
            if (realRange(gen) < p)
            {
                element = neighbour;
            }
        }
    }
    return *optElement;
}
