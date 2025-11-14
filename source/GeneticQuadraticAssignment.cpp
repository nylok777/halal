//
// Created by david on 10/5/25.
//

#include "../headers/GeneticQuadraticAssignment.h"

#include <random>
#include <unordered_set>

GeneticQuadraticAssignment::GeneticQuadraticAssignment(QuadraticAssignmentProblem& quadratic_assignment_problem)
    : quadratic_assignment_problem(std::move(quadratic_assignment_problem)) {}

assignment GeneticQuadraticAssignment::CrossOver(const std::vector<assignment>& parents)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> rand_parent(0, static_cast<int>(parents.size()) - 1);
    std::uniform_int_distribution<> rand_elem(0, static_cast<int>(parents[0].rep.size()) - 1);
    auto child = std::vector<int>(parents[0].rep.size());
    auto used = std::unordered_set<int>();
    for (auto& c : child) {
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
    return assignment{child, quadratic_assignment_problem.Objective(child)};
}

void GeneticQuadraticAssignment::Mutate(assignment& child)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, static_cast<int>(child.rep.size()) - 1);

    std::swap(child.rep[dist(gen)], child.rep[dist(gen)]);
    child.score = quadratic_assignment_problem.Objective(child.rep);
}

assignment GeneticQuadraticAssignment::GetBest(const std::vector<assignment>& population)
{
    const auto best = std::ranges::min_element(population,
                                               [](const auto& lhs, const auto& rhs) { return lhs.score < rhs.score; });
    return *best;
}

assignment GeneticQuadraticAssignment::GenerateInstance()
{
    auto instance = quadratic_assignment_problem.GenerateInstance();
    const auto score = quadratic_assignment_problem.Objective(instance);
    return assignment{std::move(instance), score};
}