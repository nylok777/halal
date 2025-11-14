//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICQUADRATICASSIGNMENT_H
#define HALAL_GENETICQUADRATICASSIGNMENT_H
#include "GeneticSolvable.h"
#include "QuadraticAssignmentProblem.h"

class GeneticQuadraticAssignment final : public GeneticSolvable<assignment>
{
public:
    explicit GeneticQuadraticAssignment(QuadraticAssignmentProblem& quadratic_assignment_problem);

    assignment CrossOver(const std::vector<assignment>&) override;

    void Mutate(assignment&) override;

    assignment GetBest(const std::vector<assignment>&) override;

    assignment GenerateInstance() override;

private:
    QuadraticAssignmentProblem quadratic_assignment_problem;
};

#endif //HALAL_GENETICQUADRATICASSIGNMENT_H