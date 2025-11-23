//
// Created by david on 9/17/25.
//
#ifndef HALAL_QUADRATICASSIGNMENTPROBLEM_H
#define HALAL_QUADRATICASSIGNMENTPROBLEM_H
#include <cmath>
#include <string>
#include <vector>

#include "JobShopProblem.h"
#include "ProblemRepresentation.h"
#include "SymmetricMatrix.hpp"

struct assignment
{
    std::vector<int> rep;
    float score;
};

class QuadraticAssignmentProblem final
    : public ProblemRepresentation<std::vector<int>, float>
{
public:
    QuadraticAssignmentProblem(const std::string& filename);

    std::vector<int> GenerateInstance() const override;

    float Objective(const std::vector<int>&) const override;

    int ProblemSize() const;

private:
    int n = 0;
    SymmetricMatrix<float> weight_matrix{n};
    SymmetricMatrix<float> distance_matrix{n};
};

#endif //HALAL_QUADRATICASSIGNMENTPROBLEM_H