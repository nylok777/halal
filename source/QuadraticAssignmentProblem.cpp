//
// Created by david on 9/17/25.
//

#include "../headers/QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>
#include <fstream>

#include "../headers/JobShopProblem.h"

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const std::string& filename)
{
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    int m;
    in >> m;
    this->n = m;
    this->weight_matrix = SymmetricMatrix<float>(n);
    this->distance_matrix = SymmetricMatrix<float>(n);
    float val;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> val;
            weight_matrix(i, j) = val;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> val;
            distance_matrix(i, j) = val;
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
                bijection[i] = facility;
                used_facilities.insert(facility);
                available = false;
            }
        }
    }
    return assignment{std::move(bijection)};
}

float QuadraticAssignmentProblem::Objective(const assignment& p) const
{
    float sum = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            sum += weight_matrix(p.rep[i], p.rep[j]) * distance_matrix(i, j) * 2.0f;
        }
    }
    return sum;
}


int QuadraticAssignmentProblem::ProblemSize() const
{
    return n;
}