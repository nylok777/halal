//
// Created by david on 11/19/25.
//

#include "headers/GeneticTravelingSalesman.h"

#include <algorithm>
#include <random>
#include <ranges>

GeneticTravelingSalesman::GeneticTravelingSalesman(std::vector<location>& all_locations)
    : TravelingSalesmanProblem(all_locations) {}


route GeneticTravelingSalesman::CrossOver(const std::vector<route>& parents) const
{
    std::vector<location> child_locations;
    child_locations.reserve(NumberOfLocations());
    child_locations.push_back(parents[0].rep.front());
    const auto parent_length = static_cast<int>(NumberOfLocations() / parents.size());
    while (child_locations.size() < NumberOfLocations()) {
        for (const auto & parent : parents) {
            int length = 0;
            auto it = parent.rep.cbegin();
            for (; length <= parent_length && it != parent.rep.cend(); ++it) {
                if (std::ranges::none_of(child_locations, [&it](const auto& loc){ return it->id == loc.id; })) {
                    child_locations.push_back(*it);
                    if (child_locations.size() == NumberOfLocations()) break;
                    ++length;
                }
            }
            if (length <= parent_length && it != parent.rep.cend()) break;
        }
    }
    route child{std::move(child_locations)};
    child.score = Objective(child);
    return child;
}

void GeneticTravelingSalesman::Mutate(route& child) const
{
    const auto n_mutations = static_cast<int>(static_cast<double>(child.rep.size())*0.2);
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dist{1, static_cast<int>(child.rep.size()) - 1};
    for (int i = 0; i < n_mutations; i+=2) {
        std::swap(child.rep[dist(gen)], child.rep[dist(gen)]);
    }
}
