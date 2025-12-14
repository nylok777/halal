//
// Created by david on 9/23/25.
//

#ifndef HALAL_STOCHASTICSOLVABLE_H
#define HALAL_STOCHASTICSOLVABLE_H
#include "Solution.h"

template <Solution T>
class StochasticSolvable
{
public:
    virtual ~StochasticSolvable() = default;
    virtual auto GenerateNeighbour(const T&, float eps) const -> T = 0;
};
#endif //HALAL_STOCHASTICSOLVABLE_H