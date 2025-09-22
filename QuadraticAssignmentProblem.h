//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_QUADRATICASSIGNMENTPROBLEM_H
#define HALAL_QUADRATICASSIGNMENTPROBLEM_H
#include <cmath>
#include <string>
#include <vector>
#include "GeneticSolvable.h"

template <typename T>
struct symmetric_matrix
{
private:
    int n;
    std::vector<T> data;

    static int index(int i, int j) {
        if (i < j) std::swap(i, j);
        return i * (i + 1) / 2 + j;
    }

public:
    explicit symmetric_matrix(int size) : n(size), data(size * (size + 1) / 2) {}

    T& operator()(const int i, const int j) {
        return data[index(i, j)];
    }

    const T& operator()(const int i, const int j) const {
        return data[index(i, j)];
    }
};

class QuadraticAssignmentProblem final : public GeneticSolvable<std::vector<int>>
{
    int n = 0;
    mutable float last_fitness = std::numeric_limits<float>::max();
    mutable float current_fitness = 0.0;
    int max_drought;
    float drought_radius;
    mutable int drought_count = 0;
    symmetric_matrix<float> weight_matrix{n};
    symmetric_matrix<float> distance_matrix{n};

public:
    virtual ~QuadraticAssignmentProblem() = default;

    explicit QuadraticAssignmentProblem(int n);
    QuadraticAssignmentProblem(std::string& filename, int max_drought, float drought_radius);

    std::vector<int> GenerateElement() const override;
    std::vector<int> GenerateNeighbour(const std::vector<int>&, float) const override;

    float Objective(const std::vector<int>&) const override;
    bool StopSearch() const override;
    std::vector<int> CrossOver(const std::vector<std::vector<int>>&) override;
    std::vector<int> Mutate(std::vector<int>&) override;
    std::pair<std::vector<int>, float> GetBest(const std::vector<std::vector<int>>&, const std::vector<float>&) override;
};


#endif //HALAL_QUADRATICASSIGNMENTPROBLEM_H
