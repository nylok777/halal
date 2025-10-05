//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_STOCHASTICQUADRATICASSIGNMENT_H
#define HALAL_STOCHASTICQUADRATICASSIGNMENT_H
#include "headers/StochasticSolvable.h"
#include <vector>

#include "headers/QuadraticAssignmentProblem.h"

class StochasticQuadraticAssignment : StochasticSolvable<std::vector<int>>
{
public:
    explicit StochasticQuadraticAssignment(QuadraticAssignmentProblem quadratic_assignment_problem);

    [[nodiscard]] std::vector<int> GenerateNeighbour(const std::vector<int> &, float eps) const override;

    QuadraticAssignmentProblem &GetProblem() override;
private:
    QuadraticAssignmentProblem quadratic_assignment_problem;
};

#endif //HALAL_STOCHASTICQUADRATICASSIGNMENT_H