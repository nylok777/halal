//
// Created by david on 9/22/25.
//
#include "../headers/JobShopProblem.h"
#include "../headers/DynamicMatrix.hpp"
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

bool operator<(const operation& op1, const operation& op2)
{
    if (op1.precedence != op2.precedence) return op1.precedence < op2.precedence;
    if (op1.job_id != op2.job_id) return op1.job_id < op2.job_id;
    if (op1.machine_id != op2.machine_id) return op1.machine_id < op2.machine_id;
    if (op1.time != op2.time) return op1.time < op2.time;
    return false;
}

bool operator>(const operation& op1, const operation& op2) { return !(op1 < op2); }

bool operator==(const operation& op1, const operation& op2) { return !(op2 < op1) && !(op1 < op2); }

std::ostream& operator<<(std::ostream& stream, const operation& operation)
{
    return stream << 'J' << operation.job_id << "-" << operation.precedence << "M" << operation.machine_id;
}

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

    jobshop_schedule operator()(
        const int n_operations,
        const int n_machines,
        std::vector<operation>&& unscheduled_operations);

    jobshop_schedule operator()(
        const int n_operations,
        std::vector<operation>&& unscheduled_operations,
        DynamicMatrix<operation>& inactive_schedule);

private:
    operation_with_times SelectAndUpdate(const std::vector<operation_with_times>& conflict);

    static std::vector<operation_with_times> CreateConflictSet(const std::vector<operation_with_times>& ops_times);

    std::vector<operation_with_times> CalculateStartAndFinishForOperations(
        const std::vector<operation>& unscheduled_operations);

    std::vector<float> machine_available_time;
    std::vector<float> job_ready_time;
    std::vector<int> jobs_progress;
};

GifflerThompson::GifflerThompson(const int n_jobs, const int n_machines)
    : machine_available_time(std::vector(n_machines, 0.0f)),
      job_ready_time(std::vector(n_jobs, 0.0f)),
      jobs_progress(std::vector(n_jobs, 1)) {}

jobshop_schedule GifflerThompson::operator()(
    const int n_operations,
    const int n_machines,
    std::vector<operation>&& unscheduled_operations)
{
    DynamicMatrix < operation > schedule{n_machines};

    while (schedule.Size() < n_operations) {
        auto to_schedule = SelectAndUpdate(CreateConflictSet(
            CalculateStartAndFinishForOperations(unscheduled_operations)));
        schedule.PushBack(to_schedule.op->machine_id - 1, *to_schedule.op);

        std::erase_if(unscheduled_operations, [&to_schedule](const auto& op)
        {
            return *to_schedule.op == op;
        });
    }
    return jobshop_schedule{std::move(schedule), *std::ranges::max_element(job_ready_time)};
}

jobshop_schedule GifflerThompson::operator()(
    const int n_operations,
    std::vector<operation>&& unscheduled_operations,
    DynamicMatrix<operation>& inactive_schedule)
{
    std::mt19937 gen{std::random_device{}()};
    for (int i = 0; i < n_operations; ++i) {
        auto conflict = CreateConflictSet(
            CalculateStartAndFinishForOperations(unscheduled_operations));
        auto M_machine = conflict.front().op->machine_id - 1;
        auto J_m = std::ranges::find_if(unscheduled_operations,
                                        [M_machine](const auto& op) { return op.machine_id == M_machine; })->job_id;
        auto J_m_o = std::ranges::find_if(inactive_schedule.Row(M_machine),
                                          [J_m](const auto& op) { return op.job_id == J_m; });
        if (J_m_o != inactive_schedule.Row(M_machine).end()) {
            if (std::ranges::any_of(conflict, [&J_m_o](const auto& op_time) { return *op_time.op == *J_m_o; })) {
                std::uniform_int_distribution<> dist{0, inactive_schedule.SizeOfRow(M_machine) - 1};
                auto& op_to_swap = inactive_schedule.At(M_machine, dist(gen));
                if (std::ranges::none_of(
                    conflict,
                    [&op_to_swap](const auto& op_time) { return *op_time.op == op_to_swap; })) {
                    std::swap(*J_m_o, op_to_swap);
                }
            }
        }
        auto to_schedule = SelectAndUpdate(conflict);
        std::erase_if(unscheduled_operations, [&to_schedule](const auto& op)
        {
            return *to_schedule.op == op;
        });
    }
    return jobshop_schedule{std::move(inactive_schedule), *std::ranges::max_element(job_ready_time)};
}


operation_with_times GifflerThompson::SelectAndUpdate(const std::vector<operation_with_times>& conflict)
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dist{0, static_cast<int>(conflict.size()) - 1};

    auto selected_op = conflict[dist(gen)];
    ++jobs_progress.at(selected_op.op->job_id - 1);
    machine_available_time[selected_op.op->machine_id - 1] = selected_op.finish_time;
    job_ready_time[selected_op.op->job_id - 1] = selected_op.finish_time;
    return selected_op;
}

std::vector<operation_with_times> GifflerThompson::CreateConflictSet(const std::vector<operation_with_times>& ops_times)
{
    const auto shortest_op = std::ranges::min_element(
        ops_times,
        [](const auto& item1, const auto& item2) { return item1.finish_time < item2.finish_time; });

    std::vector<operation_with_times> conflict;

    for (const auto& op_time : ops_times) {
        if (op_time.op->machine_id == shortest_op->op->machine_id &&
            op_time.start_time < shortest_op->finish_time)
            conflict.push_back(op_time);
    }
    return conflict;
}

std::vector<operation_with_times> GifflerThompson::CalculateStartAndFinishForOperations(
    const std::vector<operation>& unscheduled_operations)
{
    auto schedulable = std::ranges::views::filter(
        unscheduled_operations,
        [&](const auto& op)
        {
            return op.precedence == jobs_progress.at(op.job_id - 1);
        });
    std::vector<operation_with_times> ops_times{};

    for (const auto& op : schedulable) {
        auto earliest_start = std::max(job_ready_time[op.job_id - 1], machine_available_time[op.machine_id - 1]);
        ops_times.emplace_back(&op, earliest_start, earliest_start + op.time);
    }
    return ops_times;
}

JobShopProblem::JobShopProblem(
    const int machines_num,
    const int jobs_num,
    std::vector<operation>& ops
)
    : operations(std::move(ops)),
      machines_num(machines_num),
      jobs_num(jobs_num) {}

JobShopProblem JobShopProblem::LoadFromFile(const std::string& path)
{
    std::ifstream file{path};
    std::string input;
    std::getline(file, input);
    auto tab = std::ranges::find(std::as_const(input), '\t');
    auto job_num = std::stoi(std::string{input.cbegin(), tab});
    auto machine_num = std::stoi(std::string{tab, input.cend()});
    std::vector < operation > operations;
    operations.reserve(job_num * machine_num);
    for (int i = 0; i < job_num; ++i) {
        int precedence = 0;
        uint8_t item = 0;
        int machine_id;
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
    return JobShopProblem{machine_num, job_num, operations};
}

float JobShopProblem::Objective(const jobshop_schedule& schedule) const
{
    std::vector jobs_ready_time(jobs_num, 0.0f);
    std::vector machines_available_time(machines_num, 0.0f);
    for (const auto& op : schedule.rep) {
        const auto start = std::max(jobs_ready_time[op.job_id - 1], machines_available_time[op.machine_id - 1]);
        const auto finish = start + op.time;
        jobs_ready_time[op.job_id - 1] = finish;
        machines_available_time[op.machine_id - 1] = finish;
    }
    return *std::ranges::max_element(jobs_ready_time);
}

jobshop_schedule JobShopProblem::GenerateInstance() const
{
    GifflerThompson gt{jobs_num, machines_num};
    return gt(
        static_cast<int>(operations.size()),
        machines_num,
        {operations.begin(), operations.end()});
}

const std::vector<operation>& JobShopProblem::GetOperations() const { return operations; }

int JobShopProblem::NumberOfMachines() const { return machines_num; }

int JobShopProblem::NumberOfOperations() const { return static_cast<int>(operations.size()); }

int JobShopProblem::NumberOfJobs() const { return jobs_num; }

jobshop_schedule ActiveScheduleFromInactive(
    const int n_operations,
    const int n_jobs,
    const int n_machines,
    std::vector<operation>&& operations,
    DynamicMatrix<operation>& schedule)
{
    GifflerThompson gt{n_jobs, n_machines};
    return gt(n_operations, std::move(operations), schedule);
}