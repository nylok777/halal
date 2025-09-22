//
// Created by david on 9/18/25.
//

#include <functional>
#include <iostream>

#include "QuadraticAssignmentProblem.h"
#include "SimulatedAnnealingSolver.h"
#include "HillClimb.h"
int main()
{
    std::string file = "els19.dat";
    auto problem = QuadraticAssignmentProblem(file, 10, 100);
    auto generate = [problem]() {return problem.GenerateElement();};
    auto random_neighbour = [problem](const auto& p, auto eps)
    {
        return problem.GenerateNeighbour(std::forward<decltype(p)>(p), std::forward<decltype(eps)>(eps));
    };
    auto fitness = [problem](const auto& p)
    {
        return problem.Objective(std::forward<decltype(p)>(p));
    };
    auto stop_condition = [problem]() {return problem.StopSearch();};
    std::cout << "Stochastic hill climb: \n" << std::endl;
    auto r = StochasticHillClimb<std::vector<int>, int>(7, generate, random_neighbour, fitness, stop_condition);
    auto solver = SimulatedAnnealingSolver(problem, 20000);
    std::cout << "Simulated annealing: \n" << std::endl;
    auto res = solver.SimulatedAnnealing(7.0, 10.0001);
    std::cout << "best result cost: \n" << res.second << std::endl;
};
