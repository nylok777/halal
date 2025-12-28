//
// Created by david on 9/18/25.
//
#include <chrono>
#include <iostream>

#include "solvers/GeneticAlgorithm.hpp"
#include "problems/GeneticJobShop.h"
#include "problems/GeneticHiringProblem.h"
#include "solvers/NSGAII.hpp"
#include "solvers/SimulatedAnnealing.hpp"
#include "problems/StochasticQuadraticAssignment.h"

void RunQuadraticAssignment()
{
    const auto quadratic_assignment = std::make_shared<StochasticQuadraticAssignment>("els19.dat");
    StopConditionMinChangeRate stop_condition{0.1f, 11};
    BoltzmannScheduleTemperature temperature{1000.0f};
    const auto start = std::chrono::high_resolution_clock::now();
    const auto [rep, score] = SimulatedAnnealing(
        quadratic_assignment.get(),
        stop_condition,
        temperature,
        7.0f,
        11.0f);
    const auto finish = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::seconds>(finish-start);
    std::cout << score << std::endl;
    std::cout << time.count() << "s" << std::endl;

}

void RunJobshopScheduling()
{
    const auto job_shop = std::make_shared<GeneticJobShop>(GeneticJobShop::LoadFromFile("la02.txt"));
    StopConditionMaxIterations stop_condition{3000};
    const auto start = std::chrono::high_resolution_clock::now();
    const auto [rep, score] = GeneticAlgorithm(
        job_shop.get(),
        stop_condition,
        2,
        100,
        10,
        50,
        0.4f);
    const auto finish = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::seconds>(finish-start);
    std::cout << score << std::endl;
    std::cout << time.count() << "s" << std::endl;

}

auto RunWorkAssignment()
{
    auto hiring_problem = std::make_shared<GeneticHiringProblem>("Salary.txt", 2);
    auto stop_cond = StopConditionMaxIterations{10000};
    auto comparator = ParetoDominanceComparator{hiring_problem.get()};

    const auto start = std::chrono::high_resolution_clock::now();

    auto res = NSGAII(
        hiring_problem.get(),
        100,
        50,
        30,
        comparator,
        2,
        0.6f,
        stop_cond);

    const auto finish = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::seconds>(finish-start);
    auto objectives = hiring_problem->GetObjectives();
    std::ranges::for_each(std::ranges::views::take(res, 5), [&objectives](const auto& sol) -> void
    {
        std::cout << "Total salary: " << objectives.front()(sol) << '\n' <<
            "Average quality: " << objectives.back()(sol) << '\n' << std::endl;
    });
    std::cout << time << std::endl;
}

auto main() -> int
{
    RunQuadraticAssignment();
    RunJobshopScheduling();
    //RunWorkAssignment();
};