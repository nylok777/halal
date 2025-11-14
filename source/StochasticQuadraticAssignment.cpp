//
// Created by david on 10/5/25.
//
#include "../headers/StochasticQuadraticAssignment.h"

#include <algorithm>
#include <random>
#include <unordered_set>
#include <utility>

StochasticQuadraticAssignment::StochasticQuadraticAssignment(QuadraticAssignmentProblem quadratic_assignment_problem)
    : quadratic_assignment_problem(std::move(quadratic_assignment_problem)) {}

assignment StochasticQuadraticAssignment::GenerateNeighbour(const assignment& p, const float eps) const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, quadratic_assignment_problem.ProblemSize() - 1);
    const auto epsilon = static_cast<int>(eps);
    std::vector<int> q = p.rep;

    std::vector<int> indices(epsilon);
    std::unordered_set<int> used{};

    for (int i = 0; i < epsilon; ++i) {
        auto idx = dist(gen);
        while (used.contains(idx)) {
            idx = dist(gen);
        }
        indices[i] = idx;
        used.insert(idx);
    }

    std::vector<int> values;
    values.reserve(indices.size());
    for (const auto idx : indices) { values.push_back(p.rep[idx]); }

    std::ranges::shuffle(values, gen);

    int j = 0;
    for (const auto index : indices) {
        int val = values.at(j);
        const auto tmp_it = std::ranges::find(q, val);
        const auto tmp_idx = std::distance(q.begin(), tmp_it);
        const auto tmp = q[index];
        q[index] = values.at(j);
        q[tmp_idx] = tmp;
        j++;
    }
    const auto q_score = quadratic_assignment_problem.Objective(q);
    return assignment{std::move(q), q_score};
}

assignment StochasticQuadraticAssignment::GenerateInstance()
{
    auto instance = quadratic_assignment_problem.GenerateInstance();
    const auto score = quadratic_assignment_problem.Objective(instance);
    return assignment{std::move(instance), score};
}