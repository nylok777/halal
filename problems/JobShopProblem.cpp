//
// Created by david on 9/22/25.
//
#include "JobShopProblem.h"
#include "../utils/DynamicMatrix.hpp"
#include <algorithm>
#include <fstream>
#include <random>
#include <utility>
#include <iostream>
#include <ranges>
#include <cstdint>
#include <sstream>

operation::operation(const int job_id, const int precedence, const int machine_id, const float time)
    : job_id(job_id),
      precedence(precedence),
      machine_id(machine_id),
      time(time) {}

auto operator<(const operation& op1, const operation& op2) -> bool
{
    if (op1.precedence != op2.precedence) return op1.precedence < op2.precedence;
    if (op1.job_id != op2.job_id) return op1.job_id < op2.job_id;
    if (op1.machine_id != op2.machine_id) return op1.machine_id < op2.machine_id;
    if (op1.time != op2.time) return op1.time < op2.time;
    return false;
}

auto operator>(const operation& op1, const operation& op2) -> bool { return !(op1 < op2); }

auto operator==(const operation& op1, const operation& op2) -> bool { return !(op2 < op1) && !(op1 < op2); }

auto operator<<(std::ostream& stream, const operation& operation) -> std::ostream&
{
    return stream << 'J' << operation.job_id << "-" << operation.precedence << "M" << operation.machine_id;
}

namespace
{
    struct operation_with_times
    {
        const operation* op;
        float start_time;
        float finish_time;
    };

    class GifflerThompson
    {
    public:
        explicit GifflerThompson(const int n_jobs, const int n_machines);

        auto operator()(
            const int n_operations,
            const int n_machines,
            std::vector<operation>&& unscheduled_operations) -> jobshop_schedule;

        auto operator()(
            const int n_operations,
            std::vector<operation>&& unscheduled_operations,
            DynamicMatrix<operation>& inactive_schedule) -> jobshop_schedule;

    private:
        auto SelectAndUpdate(const std::vector<operation_with_times>& conflict) -> operation_with_times;
        static auto CreateConflictSet(const std::vector<operation_with_times>& ops_times) -> std::vector<operation_with_times>;
        auto CalculateStartAndFinishForOperations(const std::vector<operation>& unscheduled_operations) -> std::vector<operation_with_times>;

        std::vector<float> machine_available_time;
        std::vector<float> job_ready_time;
        std::vector<int> jobs_progress;
    };

    GifflerThompson::GifflerThompson(const int n_jobs, const int n_machines)
        : machine_available_time(std::vector(n_machines, 0.0f)),
          job_ready_time(std::vector(n_jobs, 0.0f)),
          jobs_progress(std::vector(n_jobs, 1)) {}

    auto GifflerThompson::operator()(
        const int n_operations,
        const int n_machines,
        std::vector<operation>&& unscheduled_operations) -> jobshop_schedule
    {
        DynamicMatrix < operation > schedule{n_machines};

        while (schedule.Size() < n_operations) {
            auto to_schedule = SelectAndUpdate(CreateConflictSet(
                CalculateStartAndFinishForOperations(unscheduled_operations)));
            schedule.PushBack(to_schedule.op->machine_id - 1, *to_schedule.op);

            std::erase_if(unscheduled_operations, [&to_schedule](const auto& op)
            -> auto {
                return *to_schedule.op == op;
            });
        }
        return jobshop_schedule{std::move(schedule), *std::ranges::max_element(job_ready_time)};
    }

    auto GifflerThompson::operator()(
        const int n_operations,
        std::vector<operation>&& unscheduled_operations,
        DynamicMatrix<operation>& inactive_schedule) -> jobshop_schedule
    {
        std::mt19937 gen{std::random_device{}()};
        for (int i = 0; i < n_operations; ++i) {
            auto conflict = CreateConflictSet(
                CalculateStartAndFinishForOperations(unscheduled_operations));
            auto M_machine = conflict.front().op->machine_id - 1;
            auto J_m = std::ranges::find_if(unscheduled_operations,
                                            [M_machine](const auto& op) -> auto { return op.machine_id == M_machine; })->job_id;
            auto J_m_o = std::ranges::find_if(inactive_schedule.Row(M_machine),
                                              [J_m](const auto& op) -> auto { return op.job_id == J_m; });
            if (J_m_o != inactive_schedule.Row(M_machine).end()) {
                if (std::ranges::any_of(conflict, [&J_m_o](const auto& op_time) -> auto { return *op_time.op == *J_m_o; })) {
                    std::uniform_int_distribution<> dist{0, inactive_schedule.RowSize(M_machine) - 1};
                    auto& op_to_swap = inactive_schedule.At(M_machine, dist(gen));
                    if (std::ranges::none_of(
                        conflict,
                        [&op_to_swap](const auto& op_time) -> auto { return *op_time.op == op_to_swap; })) {
                        std::swap(*J_m_o, op_to_swap);
                        }
                }
            }
            auto to_schedule = SelectAndUpdate(conflict);
            std::erase_if(unscheduled_operations, [&to_schedule](const auto& op)
            -> auto {
                return *to_schedule.op == op;
            });
        }
        return jobshop_schedule{std::move(inactive_schedule), *std::ranges::max_element(job_ready_time)};
    }

    auto GifflerThompson::SelectAndUpdate(const std::vector<operation_with_times>& conflict) -> operation_with_times
    {
        std::mt19937 gen{std::random_device{}()};
        std::uniform_int_distribution<> dist{0, static_cast<int>(conflict.size()) - 1};

        auto selected_op = conflict[dist(gen)];
        ++jobs_progress.at(selected_op.op->job_id - 1);
        machine_available_time[selected_op.op->machine_id - 1] = selected_op.finish_time;
        job_ready_time[selected_op.op->job_id - 1] = selected_op.finish_time;
        return selected_op;
    }

    auto GifflerThompson::CreateConflictSet(const std::vector<operation_with_times>& ops_times) -> std::vector<operation_with_times>
    {
        const auto shortest_op = std::ranges::min_element(
            ops_times,
            [](const auto& item1, const auto& item2) -> auto { return item1.finish_time < item2.finish_time; });

        std::vector<operation_with_times> conflict;

        for (const auto& op_time : ops_times) {
            if (op_time.op->machine_id == shortest_op->op->machine_id &&
                op_time.start_time < shortest_op->finish_time)
                conflict.push_back(op_time);
        }
        return conflict;
    }

    auto GifflerThompson::CalculateStartAndFinishForOperations(
        const std::vector<operation>& unscheduled_operations) -> std::vector<operation_with_times>
    {
        auto schedulable = std::ranges::views::filter(
            unscheduled_operations,
            [&](const auto& op)
            -> auto {
                return op.precedence == jobs_progress.at(op.job_id - 1);
            });
        std::vector<operation_with_times> ops_times{};

        for (const auto& op : schedulable) {
            auto earliest_start = std::max(job_ready_time.at(op.job_id - 1), machine_available_time.at(op.machine_id - 1));
            ops_times.emplace_back(&op, earliest_start, earliest_start + op.time);
        }
        return ops_times;
    }
}
JobShopProblem::JobShopProblem(const int machines_num, const int jobs_num, std::vector<operation>& ops)
    :
    operations(std::move(ops)),
    machines_num(machines_num),
    jobs_num(jobs_num) {}

auto JobShopProblem::LoadFromFile(const std::string& path) -> JobShopProblem
{
    std::ifstream file{path};
    std::string input;
    std::getline(file, input);
    auto tab = std::ranges::find(std::as_const(input), '\t');
    auto job_num = std::stoul(std::string{input.cbegin(), tab});
    auto machine_num = std::stoi(std::string{tab, input.cend()});
    std::vector < operation > operations;
    operations.reserve(job_num * machine_num);
    for (size_t i = 0; i < job_num; ++i) {
        int precedence = 0;
        uint8_t item = 0;
        int machine_id = 0;
        std::getline(file, input);
        std::stringstream line{input};
        std::string word;
        while (std::getline(line, word, '\t') && precedence < 5) {
            if (item == 0) {
                machine_id = std::stoi(word) + 1;
                ++item;
            } else if (item == 1) {
                operations.emplace_back(i + 1, ++precedence, machine_id, std::stof(word));
                --item;
            }
        }
    }
    return JobShopProblem{machine_num, static_cast<int>(job_num), operations};
}

auto JobShopProblem::Objective(const ScheduleMatrix& schedule) const -> float
{
    std::vector jobs_ready_time(jobs_num, 0.0f);
    std::vector machines_available_time(machines_num, 0.0f);
    for (const auto& op : schedule) {
        const auto start = std::max(jobs_ready_time.at(op.job_id - 1), machines_available_time.at(op.machine_id - 1));
        const auto finish = start + op.time;
        jobs_ready_time.at(op.job_id - 1) = finish;
        machines_available_time.at(op.machine_id - 1) = finish;
    }
    return *std::ranges::max_element(jobs_ready_time);
}

auto JobShopProblem::GenerateSolution() const -> jobshop_schedule
{
    GifflerThompson gt{jobs_num, machines_num};
    return gt(
        static_cast<int>(operations.size()),
        machines_num,
        {operations.begin(), operations.end()});
}

static auto ActiveScheduleFromInactive(
    const int n_operations,
    const int n_jobs,
    const int n_machines,
    std::vector<operation>&& operations,
    DynamicMatrix<operation>& schedule) -> jobshop_schedule
{
    GifflerThompson gt{n_jobs, n_machines};
    return gt(n_operations, std::move(operations), schedule);
}

void JobShopProblem::Mutate(jobshop_schedule& child) const
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dist{0, machines_num - 1};
    const auto machine = dist(gen);
    dist = std::uniform_int_distribution<>{0, child.genotype.RowSize(machine) - 1};
    std::swap(child.genotype.At(machine, dist(gen)), child.genotype.At(machine, dist(gen)));
    child.score = Objective(child.genotype);
}

auto JobShopProblem::CrossOver(const std::vector<jobshop_schedule>& parents) const -> jobshop_schedule
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> job_dist{1, jobs_num};

    std::vector<int> preserved_jobs;
    preserved_jobs.reserve(jobs_num / 2);

    for (int i = 0; i < jobs_num / 2; ++i) { preserved_jobs.push_back(job_dist(gen)); }

    DynamicMatrix<operation> child{machines_num};
    child.Resize(jobs_num);
    auto parent = parents.begin();
    for (const auto job_i : preserved_jobs) {
        std::vector<std::pair<int, operation>> job_i_ops;

        for (int i = 0; i < parent->genotype.RowCount(); ++i) {
            for (int j = 0; j < parent->genotype.RowSize(i); ++j) {
                if (auto op = parent->genotype.At(i, j); op.job_id == job_i) job_i_ops.emplace_back(j, op);
            }
        }
        for (const auto& [dist, op] : job_i_ops) { child.Insert(op.machine_id - 1, dist, op); }
    }
    ++parent;
    auto op_is_empty = [](const operation& op)
    -> bool {
        return op.job_id == 0 && op.time == 0.0 && op.machine_id == 0 && op.precedence == 0;
    };
    for (const auto& op : parent->genotype) {
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
        operations.size(),
        jobs_num,
        machines_num,
        {operations.begin(), operations.end()},
        child);
    return active_child;
}
