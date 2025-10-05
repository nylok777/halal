//
// Created by david on 10/5/25.
//

#include "headers/GeneticQuadraticAssignment.h"

#include <random>
#include <unordered_set>

GeneticQuadraticAssignment::GeneticQuadraticAssignment(QuadraticAssignmentProblem &quadratic_assignment_problem)
    : quadratic_assignment_problem(std::move(quadratic_assignment_problem)) {}

QuadraticAssignmentProblem& GeneticQuadraticAssignment::GetProblem() {
    return quadratic_assignment_problem;
}

std::vector<int> GeneticQuadraticAssignment::CrossOver(const std::vector<std::vector<int>>& parents) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> rand_parent(0, parents.size() - 1);
    std::uniform_int_distribution<> rand_elem(0, parents[0].size() - 1);
    auto child = std::vector<int>(parents[0].size());
    auto used = std::unordered_set<int>();
    for (auto& c : child) {
        bool available = true;
        while (available) {
            auto p_idx = rand_parent(gen);
            auto p_elem_idx = rand_elem(gen);
            if (used.find(parents[p_idx][p_elem_idx]) == used.end()) {
                c = parents[p_idx][p_elem_idx];
                used.insert(parents[p_idx][p_elem_idx]);
                available = false;
            }
        }
    }
    return child;
}

std::vector<int> GeneticQuadraticAssignment::Mutate(std::vector<int>& child) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, child.size() - 1);

    const auto rand_pos = dist(gen);
    const auto rand_pos2 = dist(gen);
    const auto tmp = child[rand_pos];
    child[rand_pos] = child[rand_pos2];
    child[rand_pos2] = tmp;
    return child;
}

std::pair<std::vector<int>, float> GeneticQuadraticAssignment::GetBest(const std::vector<std::vector<int>>& population,
    const std::vector<float>& pop_fitness) {
    auto best_idx = 0;
    for (int i = 0; i < population.size(); ++i) {
        if (pop_fitness[i] < pop_fitness[best_idx]) {
            best_idx = i;
        }
    }
    quadratic_assignment_problem.SetCurrentFitness(pop_fitness[best_idx]);
    return std::make_pair(population[best_idx], pop_fitness[best_idx]);
}
