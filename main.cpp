//
// Created by david on 9/18/25.
//

#include <vector>
#include <iostream>
#include <Solvable.h>
#include <SimulatedAnnealingSolvable.h>
#include <QuadraticAssignmentProblem.h>

int main()
{
    std::string file = "els19.dat";
    auto problem = QuadraticAssignmentProblem(file, 20000, 10, 100);
    std::cout << "Simulated annealing: \n" << std::endl;
    auto [fst, snd] = problem.SimulatedAnnealing(7, 10.001);
    std::cout << "best solution result: \n" << snd << std::endl;
}