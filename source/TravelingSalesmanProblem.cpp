//
// Created by david on 11/19/25.
//

#include "headers/TravelingSalesmanProblem.h"
#include <algorithm>
#include <random>
#include <ranges>

location::location(const int id, std::vector<distance_from_location>& distances)
    : id(id), distances(std::move(distances))
{
    std::ranges::sort(distances, [](const auto& x, const auto& y) { return x.first < y.first; });
}

TravelingSalesmanProblem::TravelingSalesmanProblem(std::vector<location>& all_locations)
    : all_locations(std::move(all_locations)){}

std::vector<location> TravelingSalesmanProblem::GenerateInstance() const
{
    std::mt19937 gen{std::random_device{}()};
    std::vector<location> locations = all_locations;
    std::shuffle(locations.begin() + 1, locations.end(), gen);
    return locations;
}

double TravelingSalesmanProblem::Objective(const std::vector<location>& locations) const
{
    double total_distance = 0.0;
    for (auto iter = locations.cbegin(); iter != locations.cend(); ++iter) {
        auto next = std::next(iter);
        if (next == locations.cend()) break;
        const auto dist = std::ranges::find_if(iter->distances, [&next](const auto x)
        {
            return x.first == next->id;
        })->second;
        total_distance += dist;
    }
    return total_distance;
}

int TravelingSalesmanProblem::NumberOfLocations() const
{
    return static_cast<int>(all_locations.size());
}
