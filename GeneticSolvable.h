//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_GENETICSOLVABLE_H
#define HALAL_GENETICSOLVABLE_H
#include <vector>
#include "Solvable.h"

template<typename T>
class GeneticSolvable : Solvable<T>
{
protected:
    ~GeneticSolvable() = default;

public:
    virtual T CrossOver(const std::vector<T>&) = 0;
    virtual T Mutate(T) = 0;
};

#endif //HALAL_GENETICSOLVABLE_H