//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_QUADRATICASSIGNMENTPROBLEM_H
#define HALAL_QUADRATICASSIGNMENTPROBLEM_H
#include <cmath>
#include <string>
#include <vector>
#include "Matrix.h"
#include "Problem.h"

class QuadraticAssignmentProblem final
    : public Problem<std::vector<int>>
{
public:
    QuadraticAssignmentProblem(const int n, const int max_drought, const float drought_radius);
    QuadraticAssignmentProblem(std::string& filename, int max_drought, float drought_radius);

    std::vector<int> GenerateElement() const override;

    float Objective(const std::vector<int>&) const override;
    bool StopCondition() const override;
    void SetCurrentFitness(float fitness) const;

    int ProblemSize() const;

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
