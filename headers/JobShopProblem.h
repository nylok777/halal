//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_JOBSHOPPROBLEM_H
#define HALAL_JOBSHOPPROBLEM_H
#include <limits>
#include <vector>

#include "DisjunctiveGraph.h"
#include "GeneticSolvable.h"
#include "Matrix.h"
#include "Problem.h"

struct operation
{
    operation() = default;
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

class JobShopProblem : public Problem<matrix<operation>>
{
public:
    virtual ~JobShopProblem() = default;
    JobShopProblem(int machines_num, int jobs_num, std::vector<operation>& ops);
    [[nodiscard]] float Objective(const matrix<operation>&) const override;
    [[nodiscard]] matrix<operation> GenerateElement() const override;
    bool StopCondition() const override;
    void SetMakespan(const float makespan) const;
    int GetMachinesNumber() const;
private:
    std::vector<operation> operations;
    int machines_num;
    int jobs_num;
    int max_drought = 5;
    float drought_radius = 100.0;
    mutable int drought_count = 0;
    mutable float current_makespan = std::numeric_limits<float>::max();
    mutable float last_makespan = 0.0;
};
#endif //HALAL_JOBSHOPPROBLEM_H