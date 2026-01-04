//
// Created by david on 9/17/25.
//
#ifndef HALAL_QUADRATICASSIGNMENTPROBLEM_H
#define HALAL_QUADRATICASSIGNMENTPROBLEM_H
#include <string>
#include <vector>

#include "JobShopProblem.h"
#include "OptimizationProblem.h"
#include "SymmetricMatrix.hpp"
#include "Chromosome.h"

struct assignment : SolutionCandidate<std::vector<int>, float> {};

class QuadraticAssignmentProblem
    : public OptimizationProblem<assignment>, public IRandomNeighbour<assignment>, public Chromosome<assignment>
{
    using Placements = std::vector<int>;
public:
    explicit QuadraticAssignmentProblem(const std::string& filename);

    [[nodiscard]] auto GenerateInstance() const -> assignment override;

    [[nodiscard]] auto Objective(const Placements& placements) const -> float override;

    [[nodiscard]] auto ProblemSize() const -> int;

    [[nodiscard]] auto GenerateNeighbour(const assignment& p, float eps) const -> assignment override;
    [[nodiscard]] auto CrossOver(const std::vector<assignment>& parents) const -> assignment override;

    void Mutate(assignment& child) const override;

private:
    int n = 0;
    SymmetricMatrix<float> weight_matrix{n};
    SymmetricMatrix<float> distance_matrix{n};
};

#endif //HALAL_QUADRATICASSIGNMENTPROBLEM_H