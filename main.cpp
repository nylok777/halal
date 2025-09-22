//
// Created by david on 9/18/25.
//

#include <iostream>

#include "QuadraticAssignmentProblem.h"
#include "SimulatedAnnealingSolver.h"

int main()
{
    std::string file = "els19.dat";
    auto problem = QuadraticAssignmentProblem(file, 5, 100);
    auto solver = SimulatedAnnealingSolver(problem, 20000);
    std::cout << "Simulated annealing: \n" << std::endl;
    auto res = solver.SimulatedAnnealing(7.0, 10.0001);
    std::cout << "best result cost: \n" << res.second << std::endl;
}
