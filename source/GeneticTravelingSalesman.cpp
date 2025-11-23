//
// Created by david on 11/19/25.
//

#include "headers/GeneticTravelingSalesman.h"

#include <algorithm>
#include <random>

GeneticTravelingSalesman::GeneticTravelingSalesman(const std::vector<location>& all_locations)
    : tsp(TravelingSalesmanProblem{all_locations}) {}


route GeneticTravelingSalesman::CrossOver(const std::vector<route>& parents)
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution subroute_length_dist{1, tsp.NumberOfLocations() / 2 - 1};
    std::vector<location> child;
    child.push_back(parents.front().rep.front());
    while (child.size() < tsp.NumberOfLocations()) {
        auto length = subroute_length_dist(gen);
        auto start = 1;
        for (const auto& [rep, score] : parents) {
            const auto child_prev_size = static_cast<int>(child.size());
            if (const auto remaining_size = tsp.NumberOfLocations() - child_prev_size; remaining_size < length) {
                length = remaining_size;
            }
            auto start_iter = child.cbegin() + start;
            std::copy_if(start_iter, start_iter + length, std::back_inserter(child), [&child](const auto& loc)
            {
                return std::ranges::none_of(child, [&loc](const auto& x){ return x.id == loc.id; });
            });
            start += start < tsp.NumberOfLocations() ? static_cast<int>(child.size()) - child_prev_size : 1;
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
