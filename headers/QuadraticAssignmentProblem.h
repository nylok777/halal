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
#include "Matrix.h"
#include "StochasticSolvable.h"

class QuadraticAssignmentProblem final : public GeneticSolvable<std::vector<int>>, public StochasticSolvable<std::vector<int>>
{
public:
    ~QuadraticAssignmentProblem() override = default;

    QuadraticAssignmentProblem(const int n, const int max_drought, const float drought_radius);
    QuadraticAssignmentProblem(std::string& filename, int max_drought, float drought_radius);

    std::vector<int> GenerateElement() const override;
    std::vector<int> GenerateNeighbour(const std::vector<int>&, float) const override;

    float Objective(const std::vector<int>&) const override;
    bool StopCondition() const override;
    std::vector<int> CrossOver(const std::vector<std::vector<int>>&) override;
    std::vector<int> Mutate(std::vector<int>&) override;
    std::pair<std::vector<int>, float> GetBest(const std::vector<std::vector<int>>&, const std::vector<float>&) override;

private:
    int n = 0;
    mutable float last_fitness = std::numeric_limits<float>::max();
    mutable float current_fitness = 0.0;
    int max_drought;
    float drought_radius;
    mutable int drought_count = 0;
    symmetric_matrix<float> weight_matrix{n};
    symmetric_matrix<float> distance_matrix{n};
};

#endif //HALAL_QUADRATICASSIGNMENTPROBLEM_H
