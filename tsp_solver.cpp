//
// Created by david on 11/20/25.
//

#include "headers/GeneticAlgorithmSolver.hpp"
#include "headers/GeneticTravelingSalesman.h"
#include "include/tsp_solver_c.h"
#include "include/tsp_solver.h"

std::vector<location> SolveTspInstance(std::vector<location>& all_delivery_points)
{
    GeneticTravelingSalesman tsp{all_delivery_points};
    GeneticAlgorithmSolver<route, StopConditionMaxIterations> solver{
        std::make_unique<GeneticTravelingSalesman>(tsp),
        StopConditionMaxIterations{200}
    };
    auto result = solver.GeneticAlgorithm(
        4,
        100,
        10,
        50,
        0.5f).rep;
    return result;
}

void SolveTspInstance_C(point* points, const int count, int* ids)
{
    std::vector<location> delivery_points;
    delivery_points.reserve(count);
    for (int i = 0; i < count; ++i) {
        const auto& distances = points[i].distances;
        std::vector<distance_from_location> distances_vec;
        distances_vec.reserve(distances.count);
        for (int j = 0; j < distances.count; ++j) {
            distances_vec.emplace_back(distances.items[j].id, distances.items[j].distance);
        }
        delivery_points.emplace_back(points[i].id, distances_vec);
    }
    const auto results = SolveTspInstance(delivery_points);
    for (int i = 0; i < count; ++i) {
        ids[i] = results[i].id;
    }
}
