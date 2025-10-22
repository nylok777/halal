//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_JOBSHOPPROBLEM_H
#define HALAL_JOBSHOPPROBLEM_H
#include <limits>
#include <vector>
#include <iosfwd>
#include "Matrix.h"
#include "Problem.h"

struct operation
{
    operation() = default;
    operation(const int job_id, const int precedence, const int machine_id, const float time);
    int job_id;
    int precedence;
    int machine_id;
    float time;
    friend bool operator<(const operation& op1, const operation& op2);
    friend std::ostream& operator<<(std::ostream& stream, const operation operation);
};

class JobShopProblem : public Problem<Matrix<operation>>
{
public:
    virtual ~JobShopProblem() = default;
    JobShopProblem(int machines_num, int jobs_num, std::vector<operation>& ops);
    [[nodiscard]] float Objective(const Matrix<operation>&) const override;
    [[nodiscard]] Matrix<operation> GenerateElement() const override;
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