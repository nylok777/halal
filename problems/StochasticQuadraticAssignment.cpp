//
// Created by david on 10/5/25.
//
#include "StochasticQuadraticAssignment.h"

#include <algorithm>
#include <random>
#include <unordered_set>
#include <utility>

auto StochasticQuadraticAssignment::GenerateNeighbour(const assignment& p, const float eps) const -> assignment
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<size_t> dist{0, static_cast<unsigned long>(ProblemSize() - 1)};
    const auto epsilon = static_cast<int>(eps);
    auto q = p;
    std::vector<size_t> indices;
    indices.reserve(epsilon);
    std::unordered_set<size_t> used{};
    for (int i = 0; i < epsilon; ++i) {
        auto idx = dist(gen);
        while (used.contains(idx)) {
            idx = dist(gen);
        }
        indices.push_back(idx);
        used.insert(idx);
    }

    std::vector<int> values;
    values.reserve(indices.size());
    for (const auto idx : indices) {
        values.push_back(p.rep.at(idx));
    }

    std::ranges::shuffle(values, gen);
    size_t j = 0;
    for (const auto idx : indices) {
        auto val = values.at(j++);
        auto tmp_it = std::ranges::find(q.rep, val);
        *tmp_it = q.rep.at(idx);
        q.rep.at(idx) = val;
    }
    q.score = Objective(q.rep);
    return q;
}
