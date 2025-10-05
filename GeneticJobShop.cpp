//
// Created by david on 10/5/25.
//
#include "headers/GeneticJobShop.h"

#include <algorithm>
#include <random>

GeneticJobShop::GeneticJobShop(const JobShopProblem& job_shop_problem) : job_shop_problem(job_shop_problem)
{

}


std::pair<ptrdiff_t, ptrdiff_t> CheckSwitchValidity(const int m_id,
                                                    const int m_num,
                                                    matrix<operation>& schedule,
                                                    operation& op)
{
  ptrdiff_t prev_pos = -1;
  ptrdiff_t next_pos = -1;
  for (int i = 0; i < m_num; ++i) {
    if (m_id == i) {
      continue;
    }
    auto& r = schedule.row(i);
    auto op_prev =
      std::find_if(r.cbegin(), r.cend(), [op](const operation& prev)
      {
        return op.job_id == prev.job_id &&
          op.precedence - prev.precedence == 1;
      });
    auto op_next =
      std::find_if(r.cbegin(), r.cend(), [op](const operation& next)
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

matrix<operation> GeneticJobShop::Mutate(matrix<operation>& schedule)
{
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
    const auto operation_position = std::distance(row.cbegin(), operation_iter);
    const auto other_position = std::distance(other_row.cbegin(), other_operation);
    if (!((operation_position < other_position && operation.precedence < other_operation.base()->precedence) ||
      (operation_position > other_position && operation.precedence > other_operation.base()->precedence)))
      return false;
  }
  return true;
}

matrix<operation> GeneticJobShop::CrossOver(
  const std::vector<matrix<operation>>& parents)
{
  std::random_device rnd;
  std::mt19937 gen{rnd()};
  std::uniform_int_distribution random_parent(0, static_cast<int>(parents.size()));

  auto child = matrix<operation>(parents[0].row_num(), parents[0].column_num());
  child.row(0) = parents[random_parent(gen)].row(0);
  const auto rows = parents[0].row_num();
  for (int i = 1; i < rows; ++i) {
    auto row_valid = false;
    decltype(child) parent;
    auto count = 0;
    while (!row_valid && count < rows) {
      parent = parents[random_parent(gen)];
      row_valid = CheckRowCompatibility(child.row(i - 1), parent.row(i));
      count++;
    }
    if (count >= rows && i > 2) i--;
    else if (i <= 1) {
      child.row(0) = parents[random_parent(gen)].row(0);
      i = 1;
    }
    else child.row(i) = parent.row(i);
  }

  return child;
}

std::pair<matrix<operation>, float> GeneticJobShop::GetBest(const std::vector<matrix<operation>>& schedules,
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
