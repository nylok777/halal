//
// Created by david on 9/17/25.
//

#include "headers/QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>
#include <algorithm>
#include <fstream>

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const int n, const int max_drought, const float drought_radius)
    : n(n), max_drought(max_drought), drought_radius(drought_radius), weight_matrix(n), distance_matrix(n) {
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
    : max_drought(max_drought), drought_radius(drought_radius) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    int m;
    in >> m;
    this->n = m;
    this->weight_matrix = symmetric_matrix<float>(n);
    this->distance_matrix = symmetric_matrix<float>(n);
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

std::vector<int> QuadraticAssignmentProblem::GenerateElement() const {
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

std::vector<int> QuadraticAssignmentProblem::GenerateNeighbour(const std::vector<int>& p, const float eps) const {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, n - 1);
    auto epsilon = static_cast<int>(eps);
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
        auto tmp_it = std::find(q.begin(), q.end(), val);
        auto tmp_idx = std::distance(q.begin(), tmp_it);
        auto tmp = q[index];
        q[index] = values.at(j);
        q[tmp_idx] = tmp;
        j++;
    }

    return q;
}

float QuadraticAssignmentProblem::Objective(const std::vector<int>& p) const {
    float sum = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            sum += weight_matrix(p[i], p[j]) * distance_matrix(i, j) * 2.0f;
        }
    }
    current_fitness = sum;
    return sum;
}

bool QuadraticAssignmentProblem::StopCondition() const {
    if (fabsf(current_fitness - last_fitness) < drought_radius) {
        drought_count++;
    }
    else {
        drought_count = 0;
    }
    last_fitness = current_fitness;
    return drought_count >= max_drought;
}

std::vector<int> QuadraticAssignmentProblem::CrossOver(const std::vector<std::vector<int>>& parents) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> rand_parent(0, parents.size() - 1);
    std::uniform_int_distribution<> rand_elem(0, parents[0].size() - 1);
    auto child = std::vector<int>(parents[0].size());
    auto used = std::unordered_set<int>();
    for (auto& c : child) {
        bool available = true;
        while (available) {
            auto p_idx = rand_parent(gen);
            auto p_elem_idx = rand_elem(gen);
            if (used.find(parents[p_idx][p_elem_idx]) == used.end()) {
                c = parents[p_idx][p_elem_idx];
                used.insert(parents[p_idx][p_elem_idx]);
                available = false;
            }
        }
    }
    return child;
}

std::vector<int> QuadraticAssignmentProblem::Mutate(std::vector<int>& child) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, child.size() - 1);

    auto rand_pos = dist(gen);
    auto rand_pos2 = dist(gen);
    auto tmp = child[rand_pos];
    child[rand_pos] = child[rand_pos2];
    child[rand_pos2] = tmp;
    return child;
}

std::pair<std::vector<int>, float> QuadraticAssignmentProblem::GetBest(const std::vector<std::vector<int>>& population,
    const std::vector<float>& pop_fitness) {
    auto best_idx = 0;
    for (int i = 0; i < population.size(); ++i) {
        if (pop_fitness[i] < pop_fitness[best_idx]) {
            best_idx = i;
        }
    }
    current_fitness = pop_fitness[best_idx];
    return std::make_pair(population[best_idx], pop_fitness[best_idx]);
}
