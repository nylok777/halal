//
// Created by david on 10/5/25.
//
#include "StochasticQuadraticAssignment.h"

#include <algorithm>
#include <random>
#include <unordered_set>
#include <utility>

StochasticQuadraticAssignment::StochasticQuadraticAssignment(QuadraticAssignmentProblem quadratic_assignment_problem)
    : quadratic_assignment_problem(std::move(quadratic_assignment_problem)) {}

QuadraticAssignmentProblem& StochasticQuadraticAssignment::GetProblem() {
    return quadratic_assignment_problem;
}


std::vector<int> StochasticQuadraticAssignment::GenerateNeighbour(const std::vector<int>& p, const float eps) const {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, quadratic_assignment_problem.ProblemSize() - 1);
    const auto epsilon = static_cast<int>(eps);
    std::vector<int> q = p;
    auto indices = std::vector<int>(epsilon);

    auto used = std::unordered_set<int>();

    for (int i = 0; i < epsilon; ++i) {
        auto idx = dist(gen);
        bool b = true;
        while (b) {
            if (used.find(idx) == used.end()) {
                indices[i] = idx;
                used.insert(idx);
                b = false;
            }
            else {
                idx = dist(gen);
            }
        }
    }

    auto values = std::vector<int>();
    for (const auto idx : indices) { values.push_back(p[idx]); }

    std::shuffle(values.begin(), values.end(), gen);

    int j = 0;
    for (const auto index : indices) {
        int val = values.at(j);
        const auto tmp_it = std::find(q.begin(), q.end(), val);
        const auto tmp_idx = std::distance(q.begin(), tmp_it);
        const auto tmp = q[index];
        q[index] = values.at(j);
        q[tmp_idx] = tmp;
        j++;
    }

    return q;
}
