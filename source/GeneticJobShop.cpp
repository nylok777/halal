//
// Created by david on 10/5/25.
//
#include "../headers/GeneticJobShop.h"

#include <algorithm>
#include <random>
#include <ranges>
#include <utility>

GeneticJobShop::GeneticJobShop(JobShopProblem job_shop_problem) : job_shop_problem(std::move(job_shop_problem)) {}

void GeneticJobShop::Mutate(jobshop_schedule& child)
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dist{0, job_shop_problem.NumberOfMachines() - 1};
    const auto machine = dist(gen);
    dist = std::uniform_int_distribution<>{0, child.rep.SizeOfRow(machine) - 1};
    std::swap(child.rep.At(machine, dist(gen)), child.rep.At(machine, dist(gen)));
    child.score = job_shop_problem.Objective(child);
}

jobshop_schedule GeneticJobShop::CrossOver(const std::vector<jobshop_schedule>& parents)
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

        for (int i = 0; i < parent->rep.RowCount(); ++i) {
            for (int j = 0; j < parent->rep.SizeOfRow(i); ++j) {
                if (auto op = parent->rep.At(i, j); op.job_id == job_i) job_i_ops.emplace_back(j, op);
            }
        }
        for (const auto& [dist, op] : job_i_ops) { child.Insert(op.machine_id - 1, dist, op); }
    }
    ++parent;
    auto op_is_empty = [](const operation& op)
    {
        return op.job_id == 0 && op.time == 0.0 && op.machine_id == 0 && op.precedence == 0;
    };
    for (const auto& op : parent->rep) {
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
    auto active_child = ActiveScheduleFromInactive(
        job_shop_problem.NumberOfOperations(),
        job_shop_problem.NumberOfJobs(),
        job_shop_problem.NumberOfMachines(),
        {job_shop_problem.GetOperations().begin(), job_shop_problem.GetOperations().end()},
        child);
    return active_child;
}

jobshop_schedule GeneticJobShop::GetBest(const std::vector<jobshop_schedule>& schedules)
{
    return *std::ranges::min_element(schedules,
                                     [](const auto& item1, const auto& item2) { return item1.score < item2.score; });
}

jobshop_schedule GeneticJobShop::GenerateInstance()
{
    return job_shop_problem.GenerateInstance();
}
