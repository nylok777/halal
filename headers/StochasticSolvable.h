//
// Created by david on 9/23/25.
//

#ifndef HALAL_STOCHASTICSOLVABLE_H
#define HALAL_STOCHASTICSOLVABLE_H
#include "Solvable.h"

template<typename T>
class StochasticSolvable : public virtual Solvable<T>
{
public:
    virtual ~StochasticSolvable() = default;
    virtual T GenerateNeighbour(const T&, float eps) const = 0;
};
#endif //HALAL_STOCHASTICSOLVABLE_H