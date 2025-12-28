//
// Created by david on 10/5/25.
//
#include "GeneticJobShop.h"

#include <algorithm>
#include <random>
#include <ranges>
#include <utility>

GeneticJobShop::GeneticJobShop(const int machines_num, const int jobs_num, std::vector<operation> operations)
    : JobShopProblem(machines_num, jobs_num, operations) {}

GeneticJobShop::GeneticJobShop(JobShopProblem&& problem) : JobShopProblem(std::move(problem)) {}

void GeneticJobShop::Mutate(jobshop_schedule& child) const
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dist{0, NumberOfMachines() - 1};
    const auto machine = dist(gen);
    dist = std::uniform_int_distribution<>{0, child.rep.RowSize(machine) - 1};
    std::swap(child.rep.At(machine, dist(gen)), child.rep.At(machine, dist(gen)));
    child.score = Objective(child.rep);
}

auto GeneticJobShop::CrossOver(const std::vector<jobshop_schedule>& parents) const -> jobshop_schedule
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> job_dist{1, NumberOfJobs()};

    std::vector<int> preserved_jobs;
    preserved_jobs.reserve(NumberOfJobs() / 2);

    for (int i = 0; i < NumberOfJobs() / 2; ++i) { preserved_jobs.push_back(job_dist(gen)); }

    DynamicMatrix<operation> child{NumberOfMachines()};
    child.Resize(NumberOfJobs());
    auto parent = parents.begin();
    for (const auto job_i : preserved_jobs) {
        std::vector<std::pair<int, operation>> job_i_ops;

        for (int i = 0; i < parent->rep.RowCount(); ++i) {
            for (int j = 0; j < parent->rep.RowSize(i); ++j) {
                if (auto op = parent->rep.At(i, j); op.job_id == job_i) job_i_ops.emplace_back(j, op);
            }
        }
        for (const auto& [dist, op] : job_i_ops) { child.Insert(op.machine_id - 1, dist, op); }
    }
    ++parent;
    auto op_is_empty = [](const operation& op)
    -> bool {
        return op.job_id == 0 && op.time == 0.0 && op.machine_id == 0 && op.precedence == 0;
    };
    for (const auto& op : parent->rep) {
        if (std::ranges::find(preserved_jobs, op.job_id) == preserved_jobs.end()) {
            const auto row = op.machine_id - 1;
            for (int i = 0; i < child.RowSize(row); ++i) {
                if (op_is_empty(child.At(row, i))) {
                    child.Insert(row, i, op);
                    break;
                }
            }
        }
    }
    auto active_child = ActiveScheduleFromInactive(
        NumberOfOperations(),
        NumberOfJobs(),
        NumberOfMachines(),
        {GetOperations().begin(), GetOperations().end()},
        child);
    return active_child;
}
