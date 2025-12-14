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
    virtual auto CrossOver(const std::vector<T>&) const -> T = 0;
    virtual void Mutate(T&) const = 0;
};

#endif //HALAL_GENETICSOLVABLE_H