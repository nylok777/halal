//
// Created by david on 9/17/25.
//

#include "QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>
#include <fstream>

#include "JobShopProblem.h"

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

auto QuadraticAssignmentProblem::GenerateInstance() const -> assignment
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


auto QuadraticAssignmentProblem::ProblemSize() const -> int
{
    return n;
}