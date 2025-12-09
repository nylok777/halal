//
// Created by david on 11/25/25.
//

#ifndef HALAL_LOCATION_H
#define HALAL_LOCATION_H
#include <utility>
#include <vector>

using distance_from_location = std::pair<int, double>;

struct location
{
    location() = default;
    location(const int id, std::vector<distance_from_location>& distances);
    int id;
    std::vector<distance_from_location> distances;
};
#endif //HALAL_LOCATION_H