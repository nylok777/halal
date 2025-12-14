//
// Created by david on 10/5/25.
//

#include "GeneticQuadraticAssignment.h"

#include <random>
#include <unordered_set>

GeneticQuadraticAssignment::GeneticQuadraticAssignment(const std::string& filename) : QuadraticAssignmentProblem(filename) {}

auto GeneticQuadraticAssignment::CrossOver(const std::vector<assignment>& parents) const -> assignment
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> rand_parent(0, static_cast<int>(parents.size()) - 1);
    std::uniform_int_distribution<> rand_elem(0, static_cast<int>(parents[0].rep.size()) - 1);
    auto child_rep = std::vector<int>(parents[0].rep.size());
    auto used = std::unordered_set<int>();
    for (auto& c : child_rep) {
        bool available = true;
        while (available) {
            auto p_idx = rand_parent(gen);
            auto p_elem_idx = rand_elem(gen);
            if (!used.contains(parents[p_idx].rep[p_elem_idx])) {
                c = parents[p_idx].rep[p_elem_idx];
                used.insert(parents[p_idx].rep[p_elem_idx]);
                available = false;
            }
        }
    }
    auto child = assignment{std::move(child_rep)};
    child.score = Objective(child);
    return child;
}

void GeneticQuadraticAssignment::Mutate(assignment& child) const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, static_cast<int>(child.rep.size()) - 1);

    std::swap(child.rep[dist(gen)], child.rep[dist(gen)]);
    child.score = Objective(child);
}
