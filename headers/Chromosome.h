//
// Created by david on 9/21/25.
//

#ifndef HALAL_GENETICSOLVABLE_H
#define HALAL_GENETICSOLVABLE_H
#include <vector>
#include "Solution.h"

template <typename T>
requires Solution<T> || ParetoSolution<T>
class Chromosome
{
public:
    using solution_type = T;
    virtual ~Chromosome() = default;
    virtual T CrossOver(const std::vector<T>&) const = 0;
    virtual void Mutate(T&) const = 0;
};

template<typename T>
T CrossOver(const T& parent1, const T& parent2);

#endif //HALAL_GENETICSOLVABLE_H