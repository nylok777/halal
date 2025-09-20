//
// Created by david on 9/18/25.
//

#include <iostream>
#include <QuadraticAssignmentProblem.h>

int main()
{
    const auto problem = QuadraticAssignmentProblem(20);
    auto result = problem.StochasticHillClimb(10.0);
    std::cout << result.second << std::endl;
}