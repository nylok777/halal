//
// Created by david on 9/22/25.
//
#include "headers/JobShopProblem.h"
#include "data_structures/DynamicMatrix.hpp"
#include <algorithm>
#include <random>
#include <set>
#include <utility>
#include <iostream>
#include <ranges>

operation::operation(const int job_id, const int precedence, const int machine_id, const float time)
    : job_id(job_id), precedence(precedence), machine_id(machine_id), time(time)
{
}

bool operator<(const operation& op1, const operation& op2)
{
    if (op1.precedence != op2.precedence) return op1.precedence < op2.precedence;
    if (op1.job_id != op2.job_id) return op1.job_id < op2.job_id;
    if (op1.machine_id != op2.machine_id) return op1.machine_id < op2.machine_id;
    if (op1.time != op2.time) return op1.time < op2.time;
    return false;
}

bool operator>(const operation& op1, const operation& op2)
{
    return !(op1 < op2);
}

bool operator==(const operation& op1, const operation& op2)
{
    return !(op2 < op1) && !(op1 < op2);
}


std::ostream& operator<<(std::ostream& stream, const operation& operation)
{
    stream << 'J' << operation.job_id << "-" << operation.precedence << "M" << operation.machine_id;
    return stream;
}

JobShopProblem::JobShopProblem(const int machines_num, const int jobs_num,
                               std::vector<operation>& ops)
    : operations(std::move(ops)),
      machines_num(machines_num),
      jobs_num(jobs_num)
{
}

int JobShopProblem::NumberOfOperations() const
{
    return operations.size();
}

int JobShopProblem::NumberOfJobs() const
{
    return jobs_num;
}


DynamicMatrix<operation> JobShopProblem::GenerateElement() const
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    DynamicMatrix < operation > schedule{machines_num};
    std::map<int, std::vector<operation>> ops_by_machines;
    std::map<int, std::vector<operation>> ops_by_time;
    std::map<int, int> jobs_progress;
    auto ops_copy = operations;

    //sort operations according to the machine they are assigned to
    for (int i = 0; i < machines_num; ++i)
    {
        auto i_machine_view = std::ranges::views::filter(operations, [&i](const auto& op)
        {
            return op.machine_id == i + 1;
        });
        ops_by_machines.emplace(i + 1, std::vector < operation >
        {
            i_machine_view.begin(), i_machine_view.end()
        }
        )
        ;
    }
    // all jobs start with 0 progress
    for (int i = 1; i < jobs_num + 1; ++i) jobs_progress.emplace(i, 0);

    //find machine with the most operations
    auto total_length = std::ranges::max_element(ops_by_machines, [](const auto& a, const auto& b)
    {
        return a.second.size() < b.second.size();
    })->second.size();

    for (int t = 0; t < total_length; ++t)
    {
        // add empty vector to time t of the schedule
        ops_by_time.emplace(t, std::vector < operation >
        {
        }
        )
        ;
        for (int machine_i = 0; machine_i < machines_num; ++machine_i)
        {
            //find those operations which are to be completed on the ith machine, and all it's preceding operations are completed
            auto next_choice_view = std::ranges::views::filter(ops_by_machines.find(machine_i + 1)->second,
                                                               [&jobs_progress](const auto& op)
                                                               {
                                                                   return op.precedence == jobs_progress.find(op.job_id)
                                                                       ->second
                                                                       + 1;
                                                               });

            //can't shuffle view
            std::vector < operation > next_choice{next_choice_view.begin(), next_choice_view.end()};

            //if (next_choice.empty()) break;

            std::ranges::shuffle(next_choice, gen);

            decltype(next_choice.begin()) next_iter;
            for (next_iter = next_choice.begin(); next_iter != next_choice.end(); ++next_iter)
            {
                auto next = *next_iter;

                //only one operation per job can be worked on at a time
                if (std::ranges::all_of(ops_by_time.find(t)->second,
                                        [&next](const auto& op) { return op.job_id != next.job_id; }))
                {
                    ops_by_time.find(t)->second.push_back(next);

                    // add operation to schedule
                    schedule.PushBack(machine_i, next);
                    auto erase_iter = std::ranges::find_if(
                        ops_copy, [&next](const operation& op) { return op == next; });
                    ops_copy.erase(erase_iter);
                    jobs_progress.find(next.job_id)->second++;

                    break;
                }
            }
            if (next_iter == next_choice.end())
            {
                schedule.PushBack(machine_i, operation{});
            }
        }
    }
    auto ops_per_job = operations.size() / jobs_num;
    for (const auto& progress : jobs_progress)
    {
        if (progress.second < ops_per_job)
        {
            auto j_ops = std::ranges::views::filter(ops_copy, [&progress](const operation& op)
            {
                return op.job_id == progress.first;
            });
            for (const auto& x : j_ops) schedule.PushBack(x.machine_id - 1, x);
        }
    }
    return schedule;
}

float JobShopProblem::Objective(const DynamicMatrix<operation>& schedule) const
{
    auto proc_times = std::vector(machines_num, 0.0f);
    for (int i = 0; i < machines_num; ++i)
    {
        for (const auto& row : schedule.Row(i))
        {
            proc_times[i] += row.time;
        }
    }

    const auto max = std::ranges::max_element(proc_times);

    return *max.base();
}

bool JobShopProblem::StopCondition() const
{
    if (fabsf(current_makespan - last_makespan) < drought_radius)
    {
        drought_count++;
    }
    else
    {
        drought_count = 0;
    }
    last_makespan = current_makespan;
    return drought_count >= max_drought;
}

void JobShopProblem::SetMakespan(const float makespan) const
{
    current_makespan = makespan;
}

int JobShopProblem::NumberOfMachines() const
{
    return machines_num;
}