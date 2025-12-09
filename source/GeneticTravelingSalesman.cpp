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
    std::vector<location> child;
    child.reserve(tsp.NumberOfLocations());
    child.push_back(parents[0].rep.front());
    const auto parent_length = static_cast<int>(tsp.NumberOfLocations() / parents.size());
    while (child.size() < tsp.NumberOfLocations()) {
        for (const auto & parent : parents) {
            int length = 0;
            auto it = parent.rep.cbegin();
            for (; length <= parent_length && it != parent.rep.cend(); ++it) {
                if (std::ranges::none_of(child, [&it](const auto& loc){ return it->id == loc.id; })) {
                    child.push_back(*it);
                    if (child.size() == tsp.NumberOfLocations()) break;
                    ++length;
                }
            }
            if (length <= parent_length && it != parent.rep.cend()) break;
        }
    }
    const auto score = tsp.Objective(child);
    return route{std::move(child), score};
}

route GeneticTravelingSalesman::GenerateInstance()
{
    auto instance = tsp.GenerateInstance();
    const auto score = tsp.Objective(instance);
    return route{std::move(instance), score};
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
