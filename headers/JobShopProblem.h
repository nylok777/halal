//
// Created by david on 9/22/25.
//

#ifndef HALAL_JOBSHOPPROBLEM_H
#define HALAL_JOBSHOPPROBLEM_H
#include <limits>
#include <vector>
#include <iosfwd>
#include "DynamicMatrix.hpp"
#include "ProblemRepresentation.h"

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

struct jobshop_schedule
{
    DynamicMatrix<operation> rep;
    float score = 0.0f;
    bool operator<(const jobshop_schedule& other) const { return score < other.score; }
};

class JobShopProblem : public ProblemRepresentation<jobshop_schedule, float>
{
public:
    JobShopProblem(int machines_num, int jobs_num, std::vector<operation>& ops);

    static JobShopProblem LoadFromFile(const std::string& path);

    [[nodiscard]] float Objective(const jobshop_schedule&) const override;

    [[nodiscard]] jobshop_schedule GenerateInstance() const override;

    const std::vector<operation>& GetOperations() const;

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

jobshop_schedule ActiveScheduleFromInactive(
    const int n_operations,
    const int n_jobs,
    const int n_machines,
    std::vector<operation>&& operations,
    DynamicMatrix<operation>& schedule);

#endif //HALAL_JOBSHOPPROBLEM_H