//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_QUADRATICASSIGNMENT_H
#define HALAL_QUADRATICASSIGNMENT_H
#include <cmath>
#include <vector>

template<typename T>
struct symmetric_matrix {
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

class QuadraticAssignmentProblem
{
    int n;
    mutable float last_fitness = std::numeric_limits<float>::max();
    mutable float current_fitness = 0.0;
    int max_drought;
    float drought_radius;
    mutable int drought_count = 0;
    symmetric_matrix<float> weight_matrix;
    symmetric_matrix<float> distance_matrix;

public:
    explicit QuadraticAssignmentProblem(int n);

    [[nodiscard]] std::vector<int> GenerateAssignment() const;
    [[nodiscard]] std::vector<int> GenerateNeighbour(const std::vector<int>&, const int&) const;

    [[nodiscard]] float Objective(const std::vector<int>&) const;
    bool StopSearch() const;
};


#endif //HALAL_QUADRATICASSIGNMENT_H