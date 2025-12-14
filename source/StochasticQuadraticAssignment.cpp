//
// Created by david on 10/5/25.
//
#include "../headers/StochasticQuadraticAssignment.h"

#include <algorithm>
#include <random>
#include <unordered_set>
#include <utility>

assignment StochasticQuadraticAssignment::GenerateNeighbour(const assignment& p, const float eps) const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, ProblemSize() - 1);
    const auto epsilon = static_cast<int>(eps);
    auto q = p;

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
        const auto tmp_it = std::ranges::find(q.rep, val);
        const auto tmp_idx = std::distance(q.rep.begin(), tmp_it);
        const auto tmp = q.rep[index];
        q.rep[index] = values.at(j);
        q.rep[tmp_idx] = tmp;
        j++;
    }
    q.score = Objective(q);
    return q;
}
