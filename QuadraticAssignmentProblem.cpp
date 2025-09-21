//
// Created by david on 9/17/25.
//

#include "QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>
#include <algorithm>

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const int n) : weight_matrix(n), distance_matrix(n) {
    this->n = n;
    this->max_drought = 5;
    this->drought_radius = 100;
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<float> weight_dist(0.1, 10.0);
    std::uniform_real_distribution<float> dist_dist(5.0, 40.0);

    int k = 1;
    this->weight_matrix = symmetric_matrix<float>(n);
    this->distance_matrix = symmetric_matrix<float>(n);
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

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const int n, const int temp0) : QuadraticAssignmentProblem(n) {
    this->temp0 = temp0;
}


std::vector<int> QuadraticAssignmentProblem::GenerateAssignment() const {
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

std::vector<int> QuadraticAssignmentProblem::GenerateNeighbour(const std::vector<int>& p, const int& eps) const {
    std::random_device rnd;
    std::mt19937 gen {rnd()};
    std::uniform_int_distribution<> dist(0, n-1);

    std::vector<int> q = p;
    auto indices = std::vector<int>(eps);

    const auto used = new std::unordered_set<int>();

    for (int i = 0; i < eps; ++i) {
        auto idx = dist(gen);
        bool b = true;
        while (b) {
            if (used->find(idx) == used->end()) {
                indices[i] = idx;
                used->insert(idx);
                b = false;
            } else {
                idx = dist(gen);
            }
        }
    }

    delete used;
    const auto values = new std::vector<int>();
    for (const auto idx : indices) {
        values->push_back(p[idx]);
    }

    std::shuffle(values->begin(), values->end(), gen);

    int j = 0;
    for (const auto index: indices) {
        int val = values->at(j);
        auto tmp_it = std::find(q.begin(), q.end(), val);
        auto tmp_idx = std::distance(q.begin(), tmp_it);
        auto tmp = q[index];
        q[index] = values->at(j);
        q[tmp_idx] = tmp;
        j++;
    }

    delete values;

    return q;
}


float QuadraticAssignmentProblem::Objective(const std::vector<int> &p) const {
    float sum = 0.0;
    int h = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = h; j < n; ++j) {
            std::pair<int, int> facilities;
            int count = 0;
            for (int k = 0; k < n; ++k) {
                if (k == i) {
                    facilities.first = p[k];
                    count++;
                } else if (k == j) {
                    facilities.second = p[k];
                    count++;
                }
                if (count == 2) {
                    break;
                }
            }
            sum += weight_matrix(facilities.first, facilities.second) * distance_matrix(i, j);
        }
        h++;
    }
    current_fitness = sum;
    return sum;
}

bool QuadraticAssignmentProblem::StopSearch() const {
    if (fabsf(current_fitness - last_fitness) < drought_radius) {
        drought_count++;
    } else {
        drought_count = 0;
    }
    last_fitness = current_fitness;
    return drought_count >= max_drought;
}

float QuadraticAssignmentProblem::BoltzmannScheduleTemperature(const int t) {
    return temp0 / std::log(t + 1);
}
