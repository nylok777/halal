//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICQUADRATICASSIGNMENT_H
#define HALAL_GENETICQUADRATICASSIGNMENT_H
#include "Chromosome.h"
#include "QuadraticAssignmentProblem.h"

class GeneticQuadraticAssignment final : public QuadraticAssignmentProblem, public Chromosome<assignment>
{
public:
    explicit GeneticQuadraticAssignment(const std::string& filename);

    auto CrossOver(const std::vector<assignment>&) const -> assignment override;

    void Mutate(assignment&) const override;
};

#endif //HALAL_GENETICQUADRATICASSIGNMENT_H