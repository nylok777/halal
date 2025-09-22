//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_JOBSHOPPROBLEM_H
#define HALAL_JOBSHOPPROBLEM_H
#include <vector>

#include "DisjunctiveGraph.h"
#include "Matrix.h"
#include "Solvable.h"

class JobShopProblem : public Solvable<matrix<int>>
{
public:
    virtual ~JobShopProblem() = default;
    explicit JobShopProblem(disjunctive_graph  job_graph);
    float Objective(const matrix<int>&) const override;
    matrix<int> GenerateElement() const override;
    bool StopCondition() const override;
private:
    disjunctive_graph graph;
};
#endif //HALAL_JOBSHOPPROBLEM_H