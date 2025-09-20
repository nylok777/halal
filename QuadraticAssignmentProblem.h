//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_QUADRATICASSIGNMENT_H
#define HALAL_QUADRATICASSIGNMENT_H
#include <cmath>
#include <vector>
using std::vector;

template<typename T>
float FrobeniusNorm(const vector<vector<T>>&);

vector<vector<float>> RandomPerturbationMatrix(int);

int CantorPair(int, int);

struct assignment
{
    std::pair<int, int> facilities;
    std::pair<int, int> locations;
    friend bool operator==(const assignment& obj, const assignment& other)
    {
        auto [fx1, fy1] = obj.facilities;
        auto [fx2, fy2] = other.facilities;
        auto [lx1, ly1] = obj.locations;
        auto [lx2, ly2] = other.locations;
        return obj.weightMatrixPtr[fx1][fy1] == other.weightMatrixPtr[fx2][fy2] || obj.distanceMatrixPtr[lx1][ly1] == other.distanceMatrixPtr[lx2][ly2];
    }
    friend bool operator!=(const assignment& obj, const assignment& other) {
        return !(obj == other);
    }
    assignment(const std::pair<int, int> &, const std::pair<int, int> &, const vector<vector<float>>&, const vector<vector<int>>&);
    const vector<vector<float>>* weightMatrixPtr;
    const vector<vector<int>>* distanceMatrixPtr;
};

struct quadratic_assignment
{
    vector<assignment> quad;
};

class QuadraticAssignmentProblem
{
    int n;
    vector<int> bijection;
    mutable float bestFitness = std::numeric_limits<float>::max();
    mutable float currentFitness = 0.0;
    int maxDrought;
    float droughtRadius;
    mutable int droughtCount = 0;

    vector<vector<int>> CantorPairMatrix(vector<assignment>);

public:
    explicit QuadraticAssignmentProblem(int n);


    vector<vector<float>> weightMatrix;
    vector<vector<int>> distanceMatrix;

    [[nodiscard]] quadratic_assignment GenerateAssignment() const;
    [[nodiscard]] quadratic_assignment GenerateNeighbour(const quadratic_assignment&, const float&) const;

    [[nodiscard]] float Objective(const quadratic_assignment&) const;
    bool StopSearch() const;

    std::pair<quadratic_assignment, float> StochasticHillClimb(const int&) const;
};


#endif //HALAL_QUADRATICASSIGNMENT_H