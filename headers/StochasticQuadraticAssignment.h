//
// Created by david on 10/5/25.
//
#ifndef HALAL_STOCHASTICQUADRATICASSIGNMENT_H
#define HALAL_STOCHASTICQUADRATICASSIGNMENT_H
#include "StochasticSolvable.h"
#include <vector>

#include "QuadraticAssignmentProblem.h"

class StochasticQuadraticAssignment final : public StochasticSolvable<assignment>
{
public:
    explicit StochasticQuadraticAssignment(QuadraticAssignmentProblem quadratic_assignment_problem);

    [[nodiscard]] assignment GenerateNeighbour(const assignment&, float eps) const override;

    assignment GenerateInstance() override;

private:
    QuadraticAssignmentProblem quadratic_assignment_problem;
};

#endif //HALAL_STOCHASTICQUADRATICASSIGNMENT_H