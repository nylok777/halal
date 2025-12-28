//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICQUADRATICASSIGNMENT_H
#define HALAL_GENETICQUADRATICASSIGNMENT_H
#include "../interfaces-and-templates/Chromosome.h"
#include "QuadraticAssignmentProblem.h"

class GeneticQuadraticAssignment final : public QuadraticAssignmentProblem, public Chromosome<assignment>
{
public:
    explicit GeneticQuadraticAssignment(const std::string& filename);

    [[nodiscard]] auto CrossOver(const std::vector<assignment>& parents) const -> assignment override;

    void Mutate(assignment& child) const override;
};

#endif //HALAL_GENETICQUADRATICASSIGNMENT_H