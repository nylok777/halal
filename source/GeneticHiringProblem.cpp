//
// Created by david on 12/12/25.
//

#include "GeneticHiringProblem.h"

#include <algorithm>
#include <random>

GeneticHiringProblem::GeneticHiringProblem(const std::string& filename, const int n_people_to_hire)
    : HiringProblem(filename, n_people_to_hire) {}

candidate_selection GeneticHiringProblem::CrossOver(const std::vector<candidate_selection>& parents) const
{
    std::mt19937 gen{std::random_device{}()};
    std::vector<int> new_candidates;
    while (new_candidates.size() == 0) {
        for (const auto & parent : parents) {
            std::uniform_int_distribution<size_t> dist{0, parent.candidates.size() - 1};
            auto chosen = parent.candidates.at(dist(gen));
            if (std::ranges::find(new_candidates, chosen) == std::ranges::end(new_candidates)) {
                new_candidates.emplace_back(chosen);
            }
        }
    }
    return candidate_selection{std::move(new_candidates)};
}

void GeneticHiringProblem::Mutate(candidate_selection& child) const
{
    auto [min, max] = std::ranges::minmax_element(child.candidates, std::less<>());
    ++min; --max;
}
