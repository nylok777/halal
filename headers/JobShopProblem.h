//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_JOBSHOPPROBLEM_H
#define HALAL_JOBSHOPPROBLEM_H
#include <limits>
#include <vector>
#include <iosfwd>
#include "data_structures/DynamicMatrix.hpp"
#include "Problem.h"

struct operation
{
    operation() = default;
    operation(const int job_id, const int precedence, const int machine_id, const float time);
    int job_id = 0;
    int precedence = 0;
    int machine_id = 0;
    float time = 0.0;
    friend bool operator<(const operation& op1, const operation& op2);
    friend bool operator>(const operation& op1, const operation& op2);
    friend bool operator==(const operation& op1, const operation& op2);
    friend std::ostream& operator<<(std::ostream& stream, const operation& operation);
};

class JobShopProblem final : public Problem<DynamicMatrix<operation>>
{
public:
    JobShopProblem(int machines_num, int jobs_num, std::vector<operation>& ops);
    [[nodiscard]] float Objective(const DynamicMatrix<operation>&) const override;
    [[nodiscard]] DynamicMatrix<operation> GenerateElement() const override;
    bool StopCondition() const override;
    void SetMakespan(const float makespan) const;
    int NumberOfMachines() const;
    int NumberOfOperations() const;
    int NumberOfJobs() const;

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