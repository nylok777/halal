//
// Created by david on 9/21/25.
//

#ifndef HALAL_GENETICSOLVABLE_H
#define HALAL_GENETICSOLVABLE_H
#include <vector>

#include "Problem.h"

template <typename T>
class GeneticSolvable
{
public:
    virtual ~GeneticSolvable() = default;
    virtual T CrossOver(const std::vector<T>&) = 0;
    virtual T Mutate(T&) = 0;
    virtual std::pair<T, float> GetBest(const std::vector<T>&, const std::vector<float>&) = 0;
    virtual Problem<T>& GetProblem() = 0;
};

#endif //HALAL_GENETICSOLVABLE_H