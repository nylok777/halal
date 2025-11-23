//
// Created by david on 11/20/25.
//

#ifndef HALAL_TSP_SOLVER_H
#define HALAL_TSP_SOLVER_H
#include <vector>
#include "headers/TravelingSalesmanProblem.h"

using distance_from_location = std::pair<int, double>;

struct delivery_point
{
    int id;
    std::forward_list<distance_from_location> distances;
    explicit operator location() const;
};

void SolveTspInstance(std::vector<delivery_point>& all_delivery_points);

#endif //HALAL_TSP_SOLVER_H