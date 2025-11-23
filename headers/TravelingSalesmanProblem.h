//
// Created by david on 11/19/25.
//

#ifndef HALAL_TRAVELINGSALESMANPROBLEM_H
#define HALAL_TRAVELINGSALESMANPROBLEM_H
#include <forward_list>
#include <vector>

#include "ProblemRepresentation.h"

using distance_from_location = std::pair<int, double>;

struct location
{
    location(const int id, const std::forward_list<distance_from_location>& distances);
    int id;
    std::forward_list<distance_from_location> distances;
};


class TravelingSalesmanProblem final : public ProblemRepresentation<std::vector<location>, double>
{
public:
    explicit TravelingSalesmanProblem(const std::vector<location>& all_locations);
    [[nodiscard]] std::vector<location> GenerateInstance() const override;
    [[nodiscard]] double Objective(const std::vector<location>&) const override;
    [[nodiscard]] int NumberOfLocations() const;
private:
    std::vector<location> all_locations;
};

#endif //HALAL_TRAVELINGSALESMANPROBLEM_H