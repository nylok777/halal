//
// Created by david on 9/17/25.
//

#include "headers/QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>
#include <fstream>

#include "headers/JobShopProblem.h"

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const int n, const int max_drought, const float drought_radius)
    : n(n), max_drought(max_drought), drought_radius(drought_radius), weight_matrix(n), distance_matrix(n)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<float> weight_dist(0.1, 10.0);
    std::uniform_real_distribution<float> dist_dist(5.0, 40.0);

    int k = 1;
    for (int i = 0; i < n; ++i) {
        weight_matrix(i, i) = 0;
        distance_matrix(i, i) = 0;
        for (int j = k; j < n; ++j) {
            weight_matrix(i, j) = weight_dist(gen);
            distance_matrix(i, j) = dist_dist(gen);
        }
        k++;
    }
}

QuadraticAssignmentProblem::QuadraticAssignmentProblem(std::string& filename, int max_drought, float drought_radius)
    : max_drought(max_drought), drought_radius(drought_radius)
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

std::vector<int> QuadraticAssignmentProblem::GenerateElement() const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, this->n - 1);

    std::unordered_set<int> used_facilities;
    auto bijection = std::vector<int>(n);
    for (int i = 0; i < n; ++i) {
        bool available = true;
        while (available) {
            auto facility = dist(gen);
            if (used_facilities.find(facility) == used_facilities.end()) {
                bijection[i] = facility;
                used_facilities.insert(facility);
                available = false;
            }
        }
    }
    return bijection;
}

float QuadraticAssignmentProblem::Objective(const std::vector<int>& p) const
{
    float sum = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            sum += weight_matrix(p[i], p[j]) * distance_matrix(i, j) * 2.0f;
        }
    }
    current_fitness = sum;
    return sum;
}

bool QuadraticAssignmentProblem::StopCondition() const
{
    if (fabsf(current_fitness - last_fitness) < drought_radius) {
        drought_count++;
    }
    else {
        drought_count = 0;
    }
    last_fitness = current_fitness;
    return drought_count >= max_drought;
}

void QuadraticAssignmentProblem::SetCurrentFitness(float fitness) const
{
    current_fitness = fitness;
}

int QuadraticAssignmentProblem::ProblemSize() const
{
    return n;
}