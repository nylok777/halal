//
// Created by david on 9/17/25.
//
#ifndef HALAL_QUADRATICASSIGNMENTPROBLEM_H
#define HALAL_QUADRATICASSIGNMENTPROBLEM_H
#include <string>
#include <vector>

#include "JobShopProblem.h"
#include "../interfaces-and-templates/OptimizationProblem.h"
#include "../utils/SymmetricMatrix.hpp"

struct assignment
{
    using NumberType = float;
    using RepresentationType = std::vector<int>;

    std::vector<int> rep;
    float score;
};

class QuadraticAssignmentProblem
    : public OptimizationProblem<assignment, float>
{
    using Placements = std::vector<int>;
public:
    explicit QuadraticAssignmentProblem(const std::string& filename);

    [[nodiscard]] auto GenerateInstance() const -> assignment override;

    [[nodiscard]] auto Objective(const Placements& placements) const -> float override;

    [[nodiscard]] auto ProblemSize() const -> int;

private:
    int n = 0;
    SymmetricMatrix<float> weight_matrix{n};
    SymmetricMatrix<float> distance_matrix{n};
};

#endif //HALAL_QUADRATICASSIGNMENTPROBLEM_H