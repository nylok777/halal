//
// Created by david on 9/17/25.
//
#pragma once

#ifndef HALAL_QUADRATICASSIGNMENTPROBLEM_H
#define HALAL_QUADRATICASSIGNMENTPROBLEM_H
#include <cmath>
#include <string>
#include <vector>

#include "JobShopProblem.h"
#include "../data_structures/Matrix.hpp"
#include "Problem.h"
#include "data_structures/SymmetricMatrix.hpp"

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
    SymmetricMatrix<float> weight_matrix{n};
    SymmetricMatrix<float> distance_matrix{n};
};

#endif //HALAL_QUADRATICASSIGNMENTPROBLEM_H