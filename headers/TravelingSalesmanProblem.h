//
// Created by david on 11/19/25.
//

#ifndef HALAL_TRAVELINGSALESMANPROBLEM_H
#define HALAL_TRAVELINGSALESMANPROBLEM_H
#include <vector>
#include "location.h"
#include "ProblemRepresentation.h"

class TravelingSalesmanProblem final : public ProblemRepresentation<std::vector<location>, double>
{
public:
    explicit TravelingSalesmanProblem(std::vector<location>& all_locations);
    [[nodiscard]] std::vector<location> GenerateInstance() const override;
    [[nodiscard]] double Objective(const std::vector<location>&) const override;
    [[nodiscard]] int NumberOfLocations() const;
private:
    std::vector<location> all_locations;
};

#endif //HALAL_TRAVELINGSALESMANPROBLEM_H