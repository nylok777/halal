//
// Created by david on 10/5/25.
//
#ifndef HALAL_STOCHASTICQUADRATICASSIGNMENT_H
#define HALAL_STOCHASTICQUADRATICASSIGNMENT_H
#include "../interfaces-and-templates/StochasticSolvable.h"
#include <vector>

#include "QuadraticAssignmentProblem.h"

class StochasticQuadraticAssignment final : public QuadraticAssignmentProblem, public StochasticSolvable<QuadraticAssignmentProblem::SolutionType>
{
public:
    using QuadraticAssignmentProblem::QuadraticAssignmentProblem;

    [[nodiscard]] auto GenerateNeighbour(const assignment& p, float eps) const -> assignment override;
};

#endif //HALAL_STOCHASTICQUADRATICASSIGNMENT_H