//
// Created by david on 9/17/25.
//

#include "QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>
#include <fstream>

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    in >> n;
    this->weight_matrix = SymmetricMatrix<float>(n);
    this->distance_matrix = SymmetricMatrix<float>(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> weight_matrix(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> distance_matrix(i, j);
        }
    }
}

auto QuadraticAssignmentProblem::GenerateSolution() const -> assignment
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, this->n - 1);

    std::unordered_set<int> used_facilities;
    auto bijection = std::vector<int>(n);

    for (int i = 0; i < n; ++i) {
        bool available = true;
        while (available) {
            if (auto facility = dist(gen); !used_facilities.contains(facility)) {
                bijection.at(i) = facility;
                used_facilities.insert(facility);
                available = false;
            }
        }
    }
    return assignment{std::move(bijection)};
}

auto QuadraticAssignmentProblem::Objective(const Placements& placements) const -> float
{
    float sum = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            sum += weight_matrix(placements.at(i), placements.at(j)) * distance_matrix(i, j) * 2.0f;
        }
    }
    return sum;
}

auto QuadraticAssignmentProblem::GenerateNeighbour(const assignment& p, const float eps) const -> assignment
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
        values.push_back(p.genotype.at(idx));
    }

    std::ranges::shuffle(values, gen);
    size_t j = 0;
    for (const auto idx : indices) {
        auto val = values.at(j++);
        auto tmp_it = std::ranges::find(q.genotype, val);
        *tmp_it = q.genotype.at(idx);
        q.genotype.at(idx) = val;
    }
    q.score = Objective(q.genotype);
    return q;
}

auto QuadraticAssignmentProblem::ProblemSize() const -> int
{
    return n;
}
auto QuadraticAssignmentProblem::CrossOver(const std::vector<assignment>& parents) const -> assignment
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    const auto vec_size = static_cast<int>(parents.front().genotype.size());
    std::uniform_int_distribution<> rand_parent(0, static_cast<int>(parents.size()) - 1);
    std::uniform_int_distribution<> rand_elem(0, vec_size - 1);
    std::vector<int> child_rep;
    child_rep.reserve(vec_size);
    auto used = std::unordered_set<int>();
    for (int i = 0; i < vec_size; ++i) {
        bool available = true;
        while (available) {
            if (auto p_elem = parents.at(rand_parent(gen)).genotype.at(rand_elem(gen)); !used.contains(p_elem)) {
                child_rep.push_back(p_elem);
                used.insert(p_elem);
                available = false;
            }
        }
    }
    const auto score = Objective(child_rep);
    return assignment{std::move(child_rep), score};
}

void QuadraticAssignmentProblem::Mutate(assignment& child) const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, static_cast<int>(child.genotype.size()) - 1);

    std::swap(child.genotype.at(dist(gen)), child.genotype.at(dist(gen)));
    child.score = Objective(child.genotype);
}
