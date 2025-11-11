//
// Created by david on 10/5/25.
//
#include "headers/GeneticJobShop.h"

#include <algorithm>
#include <random>
#include <ranges>
#include <utility>

GeneticJobShop::GeneticJobShop(JobShopProblem job_shop_problem) : job_shop_problem(std::move(job_shop_problem)) {}

std::pair<ptrdiff_t, ptrdiff_t> CheckSwitchValidity(
    const int m_id,
    const int m_num,
    DynamicMatrix<operation>& schedule,
    operation& op
)
{
    ptrdiff_t prev_pos = -1;
    ptrdiff_t next_pos = -1;
    for (int i = 0; i < m_num; ++i) {
        if (m_id == i) continue;

        auto& row_i = schedule.Row(i);
        auto op_prev =
            std::ranges::find_if(
                std::as_const(row_i),
                [op](const operation& prev)
                {
                    return op.job_id == prev.job_id && op.precedence - prev.precedence == 1;
                });
        auto op_next = std::ranges::find_if(
            std::as_const(row_i),
            [op](const operation& next) { return op.job_id == next.job_id && op.precedence + 1 == next.precedence; });
        if (op_prev != row_i.cend()) prev_pos = std::distance(row_i.cbegin(), op_prev);
        if (op_next != row_i.cend()) next_pos = std::distance(row_i.cbegin(), op_next);
        if (next_pos != -1 && prev_pos != -1) break;
    }
    return std::make_pair(prev_pos, next_pos);
}

DynamicMatrix<operation> GeneticJobShop::Mutate(DynamicMatrix<operation>& schedule)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    bool switch_success = false;
    operation* op1 = nullptr;
    operation* op2 = nullptr;
    while (!switch_success) {
        std::uniform_int_distribution dist(0, schedule.RowCount() - 1);

        const auto machine = dist(gen);
        auto& row = schedule.Row(machine);
        dist = std::uniform_int_distribution(0, static_cast<int>(row.size()) - 1);
        const int op1_pos = dist(gen);
        const int op2_pos = dist(gen);
        op1 = &row[op1_pos];
        op2 = &row[op2_pos];
        auto [op1_prev_pos, op1_next_pos] =
            CheckSwitchValidity(machine, job_shop_problem.NumberOfMachines(), schedule, *op1);
        auto [op2_prev_pos, op2_next_pos] =
            CheckSwitchValidity(machine, job_shop_problem.NumberOfMachines(), schedule, *op2);
        if (!(op2_pos <= op1_prev_pos || op2_pos >= op1_next_pos) &&
            !(op1_pos <= op2_prev_pos || op1_pos >= op2_next_pos)) { switch_success = true; }
    }
    const auto temp = *op1;
    *op1 = *op2;
    *op2 = temp;
    return schedule;
}

DynamicMatrix<operation> GeneticJobShop::CrossOver(const std::vector<DynamicMatrix<operation>>& parents)
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> job_dist{1, job_shop_problem.NumberOfJobs()};

    std::vector<int> preserved_jobs;
    preserved_jobs.reserve(job_shop_problem.NumberOfJobs() / 2);

    for (int i = 0; i < job_shop_problem.NumberOfJobs() / 2; ++i) { preserved_jobs.push_back(job_dist(gen)); }

    DynamicMatrix<operation> child{job_shop_problem.NumberOfMachines()};
    child.Resize(job_shop_problem.NumberOfJobs());
    auto parent = parents.begin();
    for (const auto job_i : preserved_jobs) {
        std::vector<std::pair<int, operation>> job_i_ops;

        for (int i = 0; i < parent->RowCount(); ++i) {
            for (int j = 0; j < parent->SizeOfRow(i); ++j) {
                if (auto op = parent->At(i, j); op.job_id == job_i) job_i_ops.emplace_back(j, op);
            }
        }
        for (const auto& [dist, op] : job_i_ops) { child.Insert(op.machine_id - 1, dist, op); }
    }
    ++parent;
    auto op_is_empty = [](const operation& op)
    {
        return op.job_id == 0 && op.time == 0.0 && op.machine_id == 0 && op.precedence == 0;
    };
    for (const auto& op : *parent) {
        if (std::ranges::find(preserved_jobs, op.job_id) == preserved_jobs.end()) {
            const auto row = op.machine_id - 1;
            for (int i = 0; i < child.SizeOfRow(row); ++i) {
                if (op_is_empty(child.At(row, i))) {
                    child.Insert(row, i, op);
                    break;
                }
            }
        }
    }
    ActiveScheduleFromInactiveGT(job_shop_problem, child);
    return child;
}

std::pair<DynamicMatrix<operation>, float> GeneticJobShop::GetBest(
    const std::vector<DynamicMatrix<operation>>& schedules,
    const std::vector<float>& fitness)
{
    auto best = 0;
    for (int i = 1; i < fitness.size(); ++i) { if (fitness[i] < fitness[best]) best = i; }
    job_shop_problem.SetMakespan(fitness[best]);
    return std::make_pair(schedules[best], fitness[best]);
}

JobShopProblem& GeneticJobShop::GetProblem() { return job_shop_problem; }
