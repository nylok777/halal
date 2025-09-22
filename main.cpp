//
// Created by david on 9/18/25.
//
#include <iostream>
#include "QuadraticAssignmentProblem.h"
#include "GeneticSolver.h"
#include "SimulatedAnnealingSolver.h"

int main() {
    std::string file = "els19.dat";
    auto problem = QuadraticAssignmentProblem(file, 10, 100);
    auto solver = GeneticSolver<std::vector<int>>(problem);
    auto res = solver.GeneticAlgorithm(2, 15, 5);
    std::cout << res.second << std::endl;
};
