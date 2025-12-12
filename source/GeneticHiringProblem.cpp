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
    const auto candidates_size = parents.front().candidates.size();
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist{0, candidates_size - 1};
    std::vector<int> new_candidates;
    while (new_candidates.size() != candidates_size) {
        for (const auto & parent : parents) {
            auto chosen = parent.candidates.at(dist(gen));
            if (std::ranges::find(new_candidates, chosen) == std::ranges::end(new_candidates)) {
                new_candidates.emplace_back(chosen);
            }
            if (new_candidates.size() == candidates_size) break;
        }
    }
    return candidate_selection{std::move(new_candidates)};
}

void GeneticHiringProblem::Mutate(candidate_selection& child) const
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist{0, child.candidates.size() - 1};
    int elem_to_change;
    size_t idx;
    do {
        idx = dist(gen);
        elem_to_change = child.candidates.at(idx);
        if (elem_to_change == std::ranges::max(child.candidates, std::greater<int>())) {
            --elem_to_change;
        } else {
            ++elem_to_change;
        }
    } while (std::ranges::none_of(child.candidates, [elem_to_change](auto x)
    {
        return x == elem_to_change;
    }));
    child.candidates.at(idx) = elem_to_change;
}
