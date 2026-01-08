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

auto TravelingSalesmanProblem::GenerateSolution() const -> route
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

std::vector<location> operator+(const std::vector<location>& locations, const std::vector<float>& steps)
{
    const auto size = std::min(locations.size(), steps.size());
    const auto& start_point = locations.at(0);
    auto locations_copy = locations;
    for (size_t i = 1; i < size; ++i) {
        auto location = locations_copy.begin() + static_cast<long>(i);
        const auto dist_from_start = std::ranges::find_if(locations_copy.at(i).distances, [&start_point](const auto& loc)
        {
            return loc.first == start_point.id;
        })->second;
        const auto dist_with_step = dist_from_start + steps.at(i-1);
        auto min_elem = std::ranges::min_element(start_point.distances, [dist_with_step](const auto& lhs, const auto& rhs)
        {
            return std::fabs(lhs.second - dist_with_step) < std::fabs(rhs.second - dist_with_step);
        });
        auto min_location = std::ranges::find_if(locations_copy, [min_elem](const auto& loc)
        {
            return loc.id == min_elem->first;
        });
        std::iter_swap(min_location, location);
    }
    return locations_copy;
}

auto TravelingSalesmanProblem::CrossOver(const std::vector<route>& parents) const -> route
{
    std::vector<location> child_locations;
    child_locations.reserve(all_locations.size());
    child_locations.push_back(parents[0].genotype.front()); // NOLINT(*-pro-bounds-avoid-unchecked-container-access)
    const auto parent_length = static_cast<int>(all_locations.size() / parents.size());
    while (child_locations.size() < all_locations.size()) {
        for (const auto & parent : parents) {
            int length = 0;
            auto it = parent.genotype.cbegin();
            for (; length <= parent_length && it != parent.genotype.cend(); ++it) {
                if (std::ranges::none_of(child_locations, [&it](const auto& loc){ return it->id == loc.id; })) {
                    child_locations.push_back(*it);
                    if (child_locations.size() == all_locations.size()) break;
                    ++length;
                }
            }
            if (length <= parent_length && it != parent.genotype.cend()) break;
        }
    }
    route child{std::move(child_locations)};
    child.score = Objective(child.genotype);
    return child;
}

void TravelingSalesmanProblem::Mutate(route& child) const
{
    const auto n_mutations = static_cast<int>(static_cast<double>(child.genotype.size())*0.2);
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dist{1, static_cast<int>(child.genotype.size()) - 1};
    for (int i = 0; i < n_mutations; i+=2) {
        std::swap(child.genotype[dist(gen)], child.genotype[dist(gen)]); // NOLINT(*-pro-bounds-avoid-unchecked-container-access)
    }
}
