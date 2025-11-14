//
// Created by david on 9/18/25.
//
#include <iostream>
#include "headers/QuadraticAssignmentProblem.h"
#include "headers/GeneticAlgorithmSolver.hpp"
#include "headers/GeneticJobShop.h"

int main()
{
    GeneticJobShop job_shop{JobShopProblem::LoadFromFile("la02.txt")};
    auto solver = GeneticAlgorithmSolver<jobshop_schedule, StopConditionMaxIterations>(
        std::make_unique<GeneticJobShop>(job_shop),
        StopConditionMaxIterations{1000});
    const auto sol = solver.GeneticAlgorithm(
        2,
        100,
        30,
        60,
        0.7f);
    std::cout << sol.score << std::endl;
};