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
    const auto vec_size = static_cast<int>(parents.front().rep.size());
    std::uniform_int_distribution<> rand_parent(0, static_cast<int>(parents.size()) - 1);
    std::uniform_int_distribution<> rand_elem(0, vec_size - 1);
    std::vector<int> child_rep;
    child_rep.reserve(vec_size);
    auto used = std::unordered_set<int>();
    for (int i = 0; i < vec_size; ++i) {
        bool available = true;
        while (available) {
            if (auto p_elem = parents.at(rand_parent(gen)).rep.at(rand_elem(gen)); !used.contains(p_elem)) {
                child_rep.push_back(p_elem);
                used.insert(p_elem);
                available = false;
            }
        }
    }
    const auto score = Objective(child_rep);
    return assignment{std::move(child_rep), score};
}

void GeneticQuadraticAssignment::Mutate(assignment& child) const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, static_cast<int>(child.rep.size()) - 1);

    std::swap(child.rep.at(dist(gen)), child.rep.at(dist(gen)));
    child.score = Objective(child.rep);
}
