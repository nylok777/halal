//
// Created by david on 11/19/25.
//

#include "headers/GeneticTravelingSalesman.h"

#include <algorithm>
#include <random>
#include <ranges>

GeneticTravelingSalesman::GeneticTravelingSalesman(std::vector<location>& all_locations)
    : tsp(TravelingSalesmanProblem{all_locations}) {}


route GeneticTravelingSalesman::CrossOver(const std::vector<route>& parents)
{
    std::vector<location> child_locations;
    child_locations.reserve(tsp.NumberOfLocations());
    child_locations.push_back(parents[0].rep.front());
    const auto parent_length = static_cast<int>(tsp.NumberOfLocations() / parents.size());
    while (child_locations.size() < tsp.NumberOfLocations()) {
        for (const auto & parent : parents) {
            int length = 0;
            auto it = parent.rep.cbegin();
            for (; length <= parent_length && it != parent.rep.cend(); ++it) {
                if (std::ranges::none_of(child_locations, [&it](const auto& loc){ return it->id == loc.id; })) {
                    child_locations.push_back(*it);
                    if (child_locations.size() == tsp.NumberOfLocations()) break;
                    ++length;
                }
            }
            if (length <= parent_length && it != parent.rep.cend()) break;
        }
    }
    route child{std::move(child_locations)};
    child.score = tsp.Objective(child);
    return child;
}

route GeneticTravelingSalesman::GenerateInstance()
{
    return tsp.GenerateInstance();
}

route GeneticTravelingSalesman::GetBest(const std::vector<route>& population)
{
    return *std::ranges::min_element(population, [](const auto& x, const auto& y){ return x.score < y.score; });
}

void GeneticTravelingSalesman::Mutate(route& child)
{
    const auto n_mutations = static_cast<int>(static_cast<double>(child.rep.size())*0.2);
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dist{1, static_cast<int>(child.rep.size()) - 1};
    for (int i = 0; i < n_mutations; i+=2) {
        std::swap(child.rep[dist(gen)], child.rep[dist(gen)]);
    }
}
