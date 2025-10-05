//
// Created by david on 9/18/25.
//
#include <iostream>
#include "headers/QuadraticAssignmentProblem.h"
#include "headers/GeneticAlgorithmSolver.h"

int main() {
    std::string file = "els19.dat";
    auto problem = QuadraticAssignmentProblem(file, 10, 100);
    auto solver = GeneticAlgorithmSolver(problem);
    const auto res = solver.GeneticAlgorithm(2, 40, 10);
    std::cout << res.second << std::endl;

};
