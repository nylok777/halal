//
// Created by david on 9/22/25.
//
#include <utility>

#include "headers/JobShopProblem.h"

#include <random>
#include <algorithm>

JobShopProblem::JobShopProblem(const int machines_num, const int jobs_num, std::vector<operation>& ops)
    : operations(std::move(ops)), machines_num(machines_num), jobs_num(jobs_num) {}


matrix<operation> JobShopProblem::GenerateElement() const {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    auto schedule = matrix(machines_num, static_cast<int>(operations.size()),
        std::vector(machines_num, std::vector(operations.size(), operation())));

    auto completed = std::set<operation>();
    auto& todo = operations;
    auto available = std::vector<operation>();
    auto jobs_progress = std::vector(jobs_num, 1);

    while (completed.size() < operations.size()) {
        for (const auto& t : todo) {
            if (t.precedence == jobs_progress[t.job_id] && completed.find(t) == completed.end()) {
                available.push_back(t);
            }
        }
        std::shuffle(available.begin(), available.end(), gen);
        const auto& op = available.back();
        const auto& op_machine = op.machine_id;
        auto& machine_row = schedule.row(op_machine);
        for (auto & i : machine_row) {
            if (i.job_id == 0) {
                i = op;
                jobs_progress[op.job_id]++;
                completed.insert(op);
                available.pop_back();
                break;
            }
        }
    }
    return schedule;
}

float JobShopProblem::Objective(const matrix<operation>& schedule) const {
    auto proc_times = std::vector(machines_num, 0.0f);
    for (int i = 0; i < machines_num; ++i) {
        for (const auto& row : schedule.row(i)) {
            proc_times[i] += row.time;
        }
    }

    const auto max = std::max_element(proc_times.begin(), proc_times.end());

    return *max.base();
}

bool JobShopProblem::StopCondition() const {
    if (fabsf(current_makespan - last_makespan) < drought_radius) {
        drought_count++;
    }
    else {
        drought_count = 0;
    }
    last_makespan = current_makespan;
    return drought_count >= max_drought;
}

std::pair<ptrdiff_t, ptrdiff_t> CheckSwitchValidity(const int m_id, const int m_num, matrix<operation>& schedule, operation& op) {

    ptrdiff_t prev_pos = -1;
    ptrdiff_t next_pos = -1;
    for (int i = 0; i < m_num; ++i) {
        if (m_id == i) continue;
        auto& r = schedule.row(i);
        auto op_prev = std::find_if(r.cbegin(), r.cend(),
            [op](const operation& prev) {
                return op.job_id == prev.job_id && op.precedence - prev.precedence == 1;
            });
        auto op_next = std::find_if(r.cbegin(), r.cend(),
            [op](const operation& next) {
                return op.job_id == next.job_id && op.precedence + 1 == next.precedence;
            });
        if (op_prev != r.cend()) {
            prev_pos = std::distance(r.cbegin(), op_prev);
        }
        if (op_next != r.cend()) {
            next_pos = std::distance(r.cbegin(), op_next);
        }
        if (next_pos != -1 && prev_pos != -1) break;
    }
    return std::make_pair(prev_pos, next_pos);
}

matrix<operation> JobShopProblem::Mutate(matrix<operation> & schedule) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    bool switch_success = false;
    operation* op1 = nullptr;
    operation* op2 = nullptr;
    while (!switch_success) {

        std::uniform_int_distribution dist(0, schedule.row_num() - 1);

        const auto machine = dist(gen);
        auto& row = schedule.row(machine);
        dist = std::uniform_int_distribution(0, static_cast<int>(row.size()) - 1);
        const int op1_pos = dist(gen);
        const int op2_pos = dist(gen);
        op1 = &row[dist(gen)];
        op2 = &row[dist(gen)];
        auto [op1_prev_pos, op1_next_pos] = CheckSwitchValidity(machine, machines_num, schedule, *op1);
        auto [op2_prev_pos, op2_next_pos] = CheckSwitchValidity(machine, machines_num, schedule, *op2);
        if (!(op2_pos <= op1_prev_pos || op2_pos >= op1_next_pos) && !(op1_pos <= op2_prev_pos || op1_pos >= op2_next_pos)) {
            switch_success = true;
        }
    }
    const auto temp = *op1;
    *op1 = *op2;
    *op2 = temp;
    return schedule;
}
