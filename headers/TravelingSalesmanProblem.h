//
// Created by david on 11/19/25.
//

#ifndef HALAL_TRAVELINGSALESMANPROBLEM_H
#define HALAL_TRAVELINGSALESMANPROBLEM_H
#include <vector>
#include "OptimizationProblem.h"

using distance_from_location = std::pair<int, double>;

struct location
{
    location() = default;
    location(const int id, std::vector<distance_from_location>& distances);
    int id;
    std::vector<distance_from_location> distances;
};

struct route
{
    using NumberType = double;
    std::vector<location> rep;
    double score = 0.0;
    bool operator<(const route& other) const { return score < other.score; }
};

class TravelingSalesmanProblem : public OptimizationProblem<route, double>
{
public:
    explicit TravelingSalesmanProblem(std::vector<location>& all_locations);
    [[nodiscard]] route GenerateInstance() const override;
    [[nodiscard]] double Objective(const route&) const override;
    [[nodiscard]] int NumberOfLocations() const;
private:
    std::vector<location> all_locations;
};

#endif //HALAL_TRAVELINGSALESMANPROBLEM_H