//
// Created by david on 11/19/25.
//

#include "TravelingSalesmanProblem.h"
#include <algorithm>
#include <random>

location::location(const int id, std::vector<distance_from_location>& distances)
    : id(id), distances(std::move(distances))
{
    std::ranges::sort(distances, [](const auto& x, const auto& y) -> auto { return x.first < y.first; });
}

TravelingSalesmanProblem::TravelingSalesmanProblem(std::vector<location>& all_locations)
    : all_locations(std::move(all_locations)){}

auto TravelingSalesmanProblem::GenerateInstance() const -> route
{
    std::mt19937 gen{std::random_device{}()};
    std::vector<location> locations = all_locations;
    std::shuffle(locations.begin() + 1, locations.end(), gen);
    route route{std::move(locations)};
    return route;
}

auto TravelingSalesmanProblem::Objective(const Locations& locations) const -> double
{
    double total_distance = 0.0;
    for (auto iter = locations.cbegin(); iter != locations.cend(); ++iter) {
        auto next = std::next(iter);
        if (next == locations.cend()) break;
        const auto dist = std::ranges::find_if(iter->distances, [&next](const auto x)
        -> auto {
            return x.first == next->id;
        })->second;
        total_distance += dist;
    }
    return total_distance;
}

auto TravelingSalesmanProblem::NumberOfLocations() const -> int
{
    return static_cast<int>(all_locations.size());
}
