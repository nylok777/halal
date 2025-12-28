//
// Created by david on 9/22/25.
//

#ifndef HALAL_JOBSHOPPROBLEM_H
#define HALAL_JOBSHOPPROBLEM_H
#include <vector>
#include <iosfwd>
#include "../utils/DynamicMatrix.hpp"
#include "../interfaces-and-templates/OptimizationProblem.h"

struct operation
{
    operation() = default;
    operation(const int job_id, const int precedence, const int machine_id, const float time);
    int job_id = 0;
    int precedence = 0;
    int machine_id = 0;
    float time = 0.0;
    friend auto operator<(const operation& op1, const operation& op2) -> bool;
    friend auto operator>(const operation& op1, const operation& op2) -> bool;
    friend auto operator==(const operation& op1, const operation& op2) -> bool;
    friend auto operator<<(std::ostream& stream, const operation& operation) -> std::ostream&;
};

struct jobshop_schedule
{
    using NumberType = float;
    using RepresentationType = DynamicMatrix<operation>;

    DynamicMatrix<operation> rep;
    float score = 0.0f;
    auto operator<(const jobshop_schedule& other) const -> bool { return score < other.score; }
};

class JobShopProblem : public OptimizationProblem<jobshop_schedule, float>
{
    using ScheduleMatrix = jobshop_schedule::RepresentationType;
public:
    JobShopProblem(int machines_num, int jobs_num, std::vector<operation>& ops);

    static auto LoadFromFile(const std::string& path) -> JobShopProblem;

    [[nodiscard]] auto Objective(const ScheduleMatrix&) const -> float override;

    [[nodiscard]] auto GenerateInstance() const -> jobshop_schedule override;

    [[nodiscard]] auto GetOperations() const -> const std::vector<operation>&;

    [[nodiscard]] auto NumberOfMachines() const -> int;

    [[nodiscard]] auto NumberOfOperations() const -> int;

    [[nodiscard]] auto NumberOfJobs() const -> int;

private:
    std::vector<operation> operations;
    int machines_num;
    int jobs_num;
};

auto ActiveScheduleFromInactive(
    const int n_operations,
    const int n_jobs,
    const int n_machines,
    std::vector<operation>&& operations,
    DynamicMatrix<operation>& schedule) -> jobshop_schedule;

#endif //HALAL_JOBSHOPPROBLEM_H