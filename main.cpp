//
// Created by david on 9/18/25.
//
#include <iostream>
#include "headers/QuadraticAssignmentProblem.h"
#include "headers/GeneticAlgorithmSolver.hpp"
#include "headers/GeneticQuadraticAssignment.h"

int main()
{
    std::string file = "els19.dat";
    auto problem = QuadraticAssignmentProblem(file, 10, 100);
    auto solvable = GeneticQuadraticAssignment(problem);
    auto solver = GeneticAlgorithmSolver(solvable);
    const auto res = solver.GeneticAlgorithm(10, 300, 100);
    std::cout << res.second << std::endl;
};
