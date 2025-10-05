//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICQUADRATICASSIGNMENT_H
#define HALAL_GENETICQUADRATICASSIGNMENT_H
#include "GeneticSolvable.h"
#include "QuadraticAssignmentProblem.h"

class GeneticQuadraticAssignment : public GeneticSolvable<std::vector<int>>
{
public:
    virtual ~GeneticQuadraticAssignment() = default;

    explicit GeneticQuadraticAssignment(QuadraticAssignmentProblem& quadratic_assignment_problem);

    std::vector<int> CrossOver(const std::vector<std::vector<int>>&) override;

    std::vector<int> Mutate(std::vector<int>&) override;

    std::pair<std::vector<int>, float> GetBest(const std::vector<std::vector<int>>&, const std::vector<float>&) override;

    QuadraticAssignmentProblem& GetProblem() override;
private:
    QuadraticAssignmentProblem quadratic_assignment_problem;
};

#endif //HALAL_GENETICQUADRATICASSIGNMENT_H