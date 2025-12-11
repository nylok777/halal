//
// Created by david on 9/18/25.
//
#include <chrono>
#include <iostream>

#include "GeneticAlgorithmSolver.hpp"
#include "GeneticJobShop.h"
#include "SimulatedAnnealingSolver.hpp"
#include "StochasticQuadraticAssignment.h"


int main()
{
    const StochasticQuadraticAssignment quadratic_assignment{QuadraticAssignmentProblem{"els19.dat"}};
    SimulatedAnnealingSolver<assignment, StopConditionMaxIterations> solver{
        std::make_unique<StochasticQuadraticAssignment>(quadratic_assignment),
        StopConditionMaxIterations{100*10000},
        10.0
    };

    auto start = std::chrono::high_resolution_clock::now();
    const auto result = solver.SimulatedAnnealing(7.0, 11.0);
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::seconds>(finish-start);
    std::cout << result.score << std::endl;
    std::cout << time.count() << "s" << std::endl;

    const GeneticJobShop job_shop{JobShopProblem::LoadFromFile("la02.txt")};
    auto solver_genetic = GeneticAlgorithmSolver<jobshop_schedule, StopConditionMaxIterations>(
        std::make_unique<GeneticJobShop>(job_shop),
        StopConditionMaxIterations{1000});

    start = std::chrono::high_resolution_clock::now();
    const auto sol = solver_genetic.GeneticAlgorithm(
        2,
        100,
        30,
        60,
        0.7f);
    finish = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::seconds>(finish-start);
    std::cout << sol.score << std::endl;
    std::cout << time.count() << "s" << std::endl;
};