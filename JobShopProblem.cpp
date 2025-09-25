//
// Created by david on 9/22/25.
//
#include <utility>

#include "headers/JobShopProblem.h"

#include <random>
#include <algorithm>

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