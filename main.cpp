//
// Created by david on 9/18/25.
//

#include <vector>
#include <QuadraticAssignmentProblem.h>
#include <hillclimb.h>

int main()
{
    auto problem = QuadraticAssignmentProblem(20);
    auto generate = [&problem] { return problem.GenerateAssignment(); };
    auto distance = [&problem](auto &&PH1, auto &&PH2) {
        return problem.GenerateNeighbour(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    };
    auto objective = [&problem](auto && PH1) { return problem.Objective(std::forward<decltype(PH1)>(PH1)); };
    auto stop_condition = [&problem] { return problem.StopSearch(); };
    const auto result = StochasticHillClimb<std::vector<int>, int>(7, generate, distance, objective, stop_condition);
}