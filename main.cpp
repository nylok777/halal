//
// Created by david on 9/18/25.
//

#include <vector>
#include <QuadraticAssignmentProblem.h>
#include <hillclimb.h>
#include <simulated_annealing.h>

int main()
{
    auto problem = QuadraticAssignmentProblem(20, 2000);
    auto generate = [&problem] { return problem.GenerateAssignment(); };
    auto distance = [&problem](auto &&PH1, auto &&PH2) {
        return problem.GenerateNeighbour(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    };
    auto objective = [&problem](auto && PH1) { return problem.Objective(std::forward<decltype(PH1)>(PH1)); };
    auto stop_condition = [&problem] { return problem.StopSearch(); };
    std::cout << "Simulated annealing: \n" << std::endl;
    auto temperature = [&problem](auto &&PH1) { return problem.BoltzmannScheduleTemperature(std::forward<decltype(PH1)>(PH1)); };

    const auto result2 = SimulatedAnnealing<std::vector<int>, int>(7, 10.001, generate, distance, objective, stop_condition, temperature);
}