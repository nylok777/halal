//
// Created by david on 9/18/25.
//
#include <iostream>

#include "include/tsp_solver_c.h"

int main()
{
    distance_from_point distances1[] = {distance_from_point{2, 20.0}, distance_from_point{3, 5.0}};
    distance_from_point distances2[] = {distance_from_point{1, 20.0}, distance_from_point{3, 13.0}};
    distance_from_point distances3[] = {distance_from_point{1, 5.0}, distance_from_point{2, 13.0}};
    point points[] = {point{1, distances_list{2, distances1}},
        point{2, distances_list{2, distances2}}, point{3, distances_list{2, distances3}}};

    int ids[] = {0, 0, 0};
    SolveTspInstance_C(points, 3, ids);

    // GeneticJobShop job_shop{JobShopProblem::LoadFromFile("la02.txt")};
    // auto solver = GeneticAlgorithmSolver<jobshop_schedule, StopConditionMaxIterations>(
    //     std::make_unique<GeneticJobShop>(job_shop),
    //     StopConditionMaxIterations{1000});
    // const auto sol = solver.GeneticAlgorithm(
    //     2,
    //     100,
    //     30,
    //     60,
    //     0.7f);
    // std::cout << sol.score << std::endl;
};