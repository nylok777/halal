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

struct operation
{
    int precedence = 0;
    int job_id = 0;
    int machine_id = 0;
    float time = 0.0;
    friend bool operator<(const operation& op1, const operation& op2) {
        if (op1.precedence != op2.precedence) {
            return op1.precedence < op2.precedence;
        }
        return op1.job_id < op2.job_id;
    }
};

class JobShopProblem : public Solvable<matrix<operation>>
{
public:
    virtual ~JobShopProblem() = default;
    float Objective(const matrix<operation>&) const override;
    [[nodiscard]] matrix<operation> GenerateElement() const override;
    bool StopCondition() const override;
private:
    std::vector<operation> operations;
    int machines_num;
    int jobs_num;
};
#endif //HALAL_JOBSHOPPROBLEM_H