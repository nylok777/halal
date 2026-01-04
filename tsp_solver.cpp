//
// Created by david on 11/20/25.
//

#include "solvers/GeneticAlgorithm.hpp"
#include "problems/GeneticTravelingSalesman.h"
#include "include/tsp_solver_c.h"
#include "include/tsp_solver.h"

#include <memory>

auto SolveTspInstance(std::vector<location>& all_delivery_points) -> std::vector<location>
{
    const auto tsp = std::make_shared<GeneticTravelingSalesman>(all_delivery_points);
    StopConditionMaxIterations stop_cond{200};
    auto result = GeneticAlgorithm(
        tsp.get(),
        stop_cond,
        4,
        100,
        10,
        50,
        0.5f).genotype;
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
