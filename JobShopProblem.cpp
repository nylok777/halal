//
// Created by david on 9/22/25.
//
#include "headers/JobShopProblem.h"
#include "data_structures/DynamicMatrix.hpp"
#include <algorithm>
#include <fstream>
#include <random>
#include <set>
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
    stream << 'J' << operation.job_id << "-" << operation.precedence << "M" << operation.machine_id;
    return stream;
}

JobShopProblem::JobShopProblem(
    const int machines_num,
    const int jobs_num,
    std::vector<operation>& ops
)
    : operations(std::move(ops)),
      machines_num(machines_num),
      jobs_num(jobs_num) {}

int JobShopProblem::NumberOfOperations() const { return static_cast<int>(operations.size()); }

int JobShopProblem::NumberOfJobs() const { return jobs_num; }

int JobShopProblem::NumberOfMachines() const { return machines_num; }

const std::vector<operation>& JobShopProblem::GetOperations() const { return operations; }

void JobShopProblem::SetMakespan(const float makespan) const { current_makespan = makespan; }

JobShopProblem JobShopProblem::LoadFromFile(const std::string& path)
{
    std::ifstream file{path};
    std::string input;
    std::getline(file, input);
    auto tab = std::ranges::find(std::as_const(input), '\t');
    auto job_num = std::stoi(std::string{input.cbegin(), tab});
    auto machine_num = std::stoi(std::string{tab, input.cend()});
    std::vector<operation> operations;
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
            }
            else if (item == 1) {
                operations.emplace_back(i + 1, ++precedence, machine_id, std::stof(word));
                --item;
            }
        }
    }
    return JobShopProblem{machine_num, job_num, operations};
}

DynamicMatrix<operation> JobShopProblem::GenerateElement() const
{
    std::mt19937 gen{std::random_device{}()};
    DynamicMatrix<operation> schedule{machines_num};
    std::vector<int> jobs_progress(jobs_num, 1);
    std::vector<operation> unscheduled_ops{operations.begin(), operations.end()};
    while (schedule.Size() != operations.size()) {
        auto schedulable = std::ranges::views::filter(
            unscheduled_ops,
            [&jobs_progress](const auto& op) { return op.precedence == jobs_progress.at(op.job_id - 1); });
        auto shortest_op = std::ranges::min_element(
            schedulable,
            [](const auto& op1, const auto& op2) { return std::fabsf(op1.time) < std::fabsf(op2.time); });
        auto conflict = std::ranges::views::filter(
            schedulable,
            [&shortest_op](const auto& op)
            {
                return op.machine_id == shortest_op->machine_id && op.precedence == shortest_op->precedence;
            });

        auto dist = std::uniform_int_distribution<int>{
            0,
            static_cast<int>(std::distance(conflict.begin(), conflict.end())) - 1
        };

        auto chosen_op = std::vector<operation>{conflict.begin(), conflict.end()}[dist(gen)];
        ++jobs_progress.at(chosen_op.job_id - 1);
        schedule.PushBack(chosen_op.machine_id - 1, chosen_op);
        std::erase(unscheduled_ops, chosen_op);
    }
    return schedule;
}

float JobShopProblem::Objective(const DynamicMatrix<operation>& schedule) const
{
    auto proc_times = std::vector(machines_num, 0.0f);
    for (int i = 0; i < machines_num; ++i) { for (const auto& row : schedule.Row(i)) { proc_times[i] += row.time; } }
    const auto max = std::ranges::max_element(proc_times);
    return *max.base();
}

bool JobShopProblem::StopCondition() const
{
    if (fabsf(current_makespan - last_makespan) < drought_radius) { drought_count++; }
    else { drought_count = 0; }
    last_makespan = current_makespan;
    return drought_count >= max_drought;
}

void ActiveScheduleFromInactiveGT(const JobShopProblem& job_shop_problem, DynamicMatrix<operation>& schedule)
{
    std::mt19937 gen{std::random_device{}()};

    std::set<operation> unscheduled_ops{
        job_shop_problem.GetOperations().begin(),
        job_shop_problem.GetOperations().end()
    };
    std::vector<int> jobs_progress(job_shop_problem.NumberOfJobs(), 1);
    while (!unscheduled_ops.empty()) {
        auto schedulable = std::ranges::views::filter(
            unscheduled_ops,
            [&jobs_progress](const auto& op) { return op.precedence == jobs_progress.at(op.job_id - 1); });
        auto shortest_op = std::ranges::min_element(
            schedulable,
            [](const auto& op1, const auto& op2) { return std::fabsf(op1.time) < std::fabsf(op2.time); });
        auto conflict_view = std::ranges::views::filter(
            schedulable,
            [&shortest_op](const auto& op)
            {
                return op.machine_id == shortest_op->machine_id && op.precedence == shortest_op->precedence;
            });

        std::set<operation> conflict{conflict_view.begin(), conflict_view.end()};

        std::uniform_int_distribution dist{0, static_cast<int>(conflict.size()) - 1};

        auto chosen_op = std::vector<operation>{conflict.begin(), conflict.end()}[dist(gen)];
        ++jobs_progress.at(chosen_op.job_id - 1);

        const auto M = shortest_op->machine_id;
        auto j_m = std::ranges::find_if(
            schedule.Row(M - 1),
            [&unscheduled_ops, &conflict](auto& op) { return unscheduled_ops.contains(op) && conflict.contains(op); });
        unscheduled_ops.erase(chosen_op);

        if (j_m == schedule.Row(M - 1).end()) continue;

        dist = std::uniform_int_distribution<int>{0, (schedule.SizeOfRow(M - 1) - 1)};

        const auto col_i = dist(gen);

        if (const auto J = schedule.At(M - 1, col_i); !conflict.contains(J)) {
            schedule.Insert(M - 1, col_i, *j_m);
            *j_m = J;
        }
    }
}
