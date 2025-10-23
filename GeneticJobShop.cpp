//
// Created by david on 10/5/25.
//
#include "headers/GeneticJobShop.h"

#include <algorithm>
#include <set>
#include <random>
#include <utility>

GeneticJobShop::GeneticJobShop(JobShopProblem job_shop_problem) : job_shop_problem(std::move(job_shop_problem)) {}


std::pair<ptrdiff_t, ptrdiff_t> CheckSwitchValidity(const int m_id,
                                                    const int m_num,
                                                    DynamicMatrix<operation>& schedule,
                                                    operation& op)
{
    ptrdiff_t prev_pos = -1;
    ptrdiff_t next_pos = -1;
    for (int i = 0; i < m_num; ++i) {
        if (m_id == i) {
            continue;
        }
        auto& r = schedule.Row(i);
        auto op_prev =
            std::ranges::find_if(std::as_const(r), [op](const operation& prev)
            {
                return op.job_id == prev.job_id &&
                    op.precedence - prev.precedence == 1;
            });
        auto op_next =
            std::ranges::find_if(std::as_const(r), [op](const operation& next)
            {
                return op.job_id == next.job_id &&
                    op.precedence + 1 == next.precedence;
            });
        if (op_prev != r.cend()) {
            prev_pos = std::distance(r.cbegin(), op_prev);
        }
        if (op_next != r.cend()) {
            next_pos = std::distance(r.cbegin(), op_next);
        }
        if (next_pos != -1 && prev_pos != -1) {
            break;
        }
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
        std::uniform_int_distribution dist(0, schedule.RowNum() - 1);

        const auto machine = dist(gen);
        auto& row = schedule.Row(machine);
        dist = std::uniform_int_distribution(0, static_cast<int>(row.size()) - 1);
        const int op1_pos = dist(gen);
        const int op2_pos = dist(gen);
        op1 = &row[dist(gen)];
        op2 = &row[dist(gen)];
        auto [op1_prev_pos, op1_next_pos] =
            CheckSwitchValidity(machine, job_shop_problem.GetMachinesNumber(), schedule, *op1);
        auto [op2_prev_pos, op2_next_pos] =
            CheckSwitchValidity(machine, job_shop_problem.GetMachinesNumber(), schedule, *op2);
        if (!(op2_pos <= op1_prev_pos || op2_pos >= op1_next_pos) &&
            !(op1_pos <= op2_prev_pos || op1_pos >= op2_next_pos)) {
            switch_success = true;
        }
    }
    const auto temp = *op1;
    *op1 = *op2;
    *op2 = temp;
    return schedule;
}

bool CheckRowCompatibility(const std::vector<operation>& row, const std::vector<operation>& other_row)
{
    for (auto operation_iter = row.cbegin(); operation_iter < row.cend(); ++operation_iter) {
        const auto operation = *operation_iter;
        auto other_operation = std::find_if(other_row.cbegin(), other_row.cend(), [operation](auto op2)
        {
            return operation.job_id == op2.job_id && operation.precedence != op2.precedence;
        });
        if (other_operation == other_row.cend()) continue;

        const auto operation_position = std::distance(row.cbegin(), operation_iter);
        const auto other_position = std::distance(other_row.cbegin(), other_operation);

        if (!((operation_position < other_position && operation.precedence < other_operation.base()->precedence) ||
            (operation_position > other_position && operation.precedence > other_operation.base()->precedence)))
            return false;
    }
    return true;
}

DynamicMatrix<operation> GeneticJobShop::CrossOver(
    const std::vector<DynamicMatrix<operation>>& parents)
{
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution random_parent(0, static_cast<int>(parents.size()) - 1);
    std::set<std::pair<int, int>> used_rows; // ith matrix jth row


    const auto n_rows = parents.begin().base()->RowNum();
    DynamicMatrix<operation> child{n_rows};
    child.Row(1) = parents[random_parent(gen)].Row(1);

    for (int i = 1; i < n_rows + 1; ++i) {
        bool compatible = false;
        DynamicMatrix<operation> parent{n_rows};
        int n_parent;
        int count = 0;
        while (!compatible && count < n_rows) {
            n_parent = random_parent(gen);
            if (used_rows.contains(std::pair(n_parent, i))) {
                continue;
            }
            parent = parents.at(n_parent);
            for (int j = 0; j < i; ++j) {
                compatible = CheckRowCompatibility(child.Row(j), parent.Row(i));
                if (!compatible) break;
            }
            count++;
        }
        if (count >= n_rows && i > 2) i--;
        else if (i == 1) {
            child.Row(1) = parents[random_parent(gen)].Row(1);
            i = 2;
        }
        else {
            child.Row(i) = parent.Row(i);
            used_rows.emplace(n_parent, i);
        }
    }

    return child;
}

std::pair<DynamicMatrix<operation>, float> GeneticJobShop::GetBest(
    const std::vector<DynamicMatrix<operation>>& schedules,
    const std::vector<float>& fitness)
{
    auto best = 0;
    for (int i = 1; i < fitness.size(); ++i) {
        if (fitness[i] < fitness[best]) best = i;
    }
    job_shop_problem.SetMakespan(fitness[best]);
    return std::make_pair(schedules[best], fitness[best]);
}

JobShopProblem& GeneticJobShop::GetProblem()
{
    return job_shop_problem;
}