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
    auto operator<(const route& other) const -> bool { return score < other.score; }
};

class TravelingSalesmanProblem : public OptimizationProblem<route, double>
{
public:
    explicit TravelingSalesmanProblem(std::vector<location>& all_locations);
    [[nodiscard]] auto GenerateInstance() const -> route override;
    [[nodiscard]] auto Objective(const route&) const -> double override;
    [[nodiscard]] auto NumberOfLocations() const -> int;
private:
    std::vector<location> all_locations;
};

#endif //HALAL_TRAVELINGSALESMANPROBLEM_H