//
// Created by david on 9/22/25.
//
#include "headers/JobShopProblem.h"
#include "headers/Matrix.h"
#include <algorithm>
#include <random>
#include <set>
#include <utility>
#include <iostream>

operation::operation(const int job_id, const int precedence, const int machine_id, const float time)
: job_id(job_id), precedence(precedence), machine_id(machine_id), time(time) {}


bool operator<(const operation& op1, const operation& op2) {
  if (op1.precedence != op2.precedence) {
    return op1.precedence < op2.precedence;
  }
  return op1.job_id < op2.job_id;
}


std::ostream& operator<<(std::ostream& stream, const operation operation)
{
  stream << "J:" << operation.job_id << " M:" << operation.machine_id << " P:" << operation.precedence;
  return stream;
}

JobShopProblem::JobShopProblem(const int machines_num, const int jobs_num,
                               std::vector<operation>& ops)
  : operations(std::move(ops)),
    machines_num(machines_num),
    jobs_num(jobs_num) {}

Matrix<operation> JobShopProblem::GenerateElement() const {
  std::random_device rnd;
  std::mt19937 gen{rnd()};
  auto schedule = Matrix<operation>(machines_num, static_cast<int>(operations.size()));

  auto completed = std::set<operation>();
  auto available = std::vector<operation>();
  auto jobs_progress = std::vector(jobs_num, 1);

  while (completed.size() < operations.size()) {
    for (const auto& t : operations) {
      if (t.precedence == jobs_progress[t.job_id-1] &&
        completed.find(t) == completed.end()) {
        available.push_back(t);
      }
    }
    std::shuffle(available.begin(), available.end(), gen);
    const auto& op = available.back();
    const auto& op_machine = op.machine_id-1;
    auto& machine_row = schedule.row(op_machine);
    for (auto& i : machine_row) {
      if (i.job_id == 0) {
        i = op;
        jobs_progress[op.job_id-1]++;
        completed.insert(op);
        available.pop_back();
        break;
      }
    }
  }
  return schedule;
}

float JobShopProblem::Objective(const Matrix<operation>& schedule) const {
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

void JobShopProblem::SetMakespan(const float makespan) const
{
  current_makespan = makespan;
}

int JobShopProblem::GetMachinesNumber() const
{
  return machines_num;
}
