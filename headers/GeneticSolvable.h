//
// Created by david on 9/21/25.
//

#ifndef HALAL_GENETICSOLVABLE_H
#define HALAL_GENETICSOLVABLE_H
#include <vector>
#include "SolutionCandidate.h"

template <SolutionCandidate T>
class GeneticSolvable
{
public:
    virtual ~GeneticSolvable() = default;
    virtual T CrossOver(const std::vector<T>&) = 0;
    virtual void Mutate(T&) = 0;
    virtual T GetBest(const std::vector<T>&) = 0;
    virtual T GenerateInstance() = 0;
};

#endif //HALAL_GENETICSOLVABLE_H