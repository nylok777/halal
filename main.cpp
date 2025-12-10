//
// Created by david on 9/18/25.
//
#include <iostream>

#include "SimulatedAnnealingSolver.hpp"
#include "StochasticQuadraticAssignment.h"
#include "include/tsp_solver_c.h"

int main()
{
    StochasticQuadraticAssignment quadratic_assignment{QuadraticAssignmentProblem{"els19.dat"}};
    SimulatedAnnealingSolver<assignment, StopConditionMinChangeRate> solver{
        std::make_unique<StochasticQuadraticAssignment>(quadratic_assignment),
        StopConditionMinChangeRate{100.0, 2},
        15.0
    };
    auto result = solver.SimulatedAnnealing(10.0, 11.0);
    std::cout << result.score << std::endl;
    // GeneticJobShop job_shop{JobShopProblem::LoadFromFile("la02.txt")};
    // auto solver = GeneticAlgorithmSolver<jobshop_schedule, StopConditionMaxIterations>(
    //     std::make_unique<GeneticJobShop>(job_shop),
    //     StopConditionMaxIterations{1000});
    // const auto sol = solver.GeneticAlgorithm(
    //     2,
    //     100,
    //     30,
    //     60,
    //     0.7f);
    // std::cout << sol.score << std::endl;
};