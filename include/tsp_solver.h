//
// Created by david on 11/25/25.
//

#ifndef HALAL_TSP_SOLVER_H
#define HALAL_TSP_SOLVER_H
#include <vector>
#include "TravelingSalesmanProblem.h"

auto SolveTspInstance(std::vector<location>& all_delivery_points) -> std::vector<location>;

#endif //HALAL_TSP_SOLVER_H
