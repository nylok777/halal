//
// Created by david on 11/19/25.
//

#ifndef HALAL_TRAVELINGSALESMANPROBLEM_H
#define HALAL_TRAVELINGSALESMANPROBLEM_H
#include <vector>
#include "../interfaces-and-templates/OptimizationProblem.h"
#include "interfaces-and-templates/Chromosome.h"

using distance_from_location = std::pair<int, double>;

struct location
{
    location() = default;
    location(const int id, std::vector<distance_from_location>& distances);
    int id{};
    std::vector<distance_from_location> distances;
};

struct route : SolutionCandidate<std::vector<location>, double>
{
    auto operator<(const route& other) const -> bool { return score < other.score; }
};

class TravelingSalesmanProblem : public OptimizationProblem<route>, public Chromosome<route>
{
    using Locations = route::RepresentationType;
public:
    explicit TravelingSalesmanProblem(std::vector<location>& all_locations);
    [[nodiscard]] auto GenerateSolution() const -> route override;
    [[nodiscard]] auto Objective(const Locations& locations) const -> double override;
    [[nodiscard]] auto CrossOver(const std::vector<route>& parents) const -> route override;
    void Mutate(route& child) const override;

private:
    std::vector<location> all_locations;
};

std::vector<location> operator+(const std::vector<location>& locations, const std::vector<float>& steps);

#endif //HALAL_TRAVELINGSALESMANPROBLEM_H