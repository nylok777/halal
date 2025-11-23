//
// Created by david on 11/20/25.
//

#include "include/tsp_solver.h"
#include "headers/GeneticAlgorithmSolver.hpp"
#include "headers/GeneticTravelingSalesman.h"
#include "include/tsp_solver_c.h"

void SolveTspInstance_C(point* points, const int count, int* ids)
{
    std::vector<delivery_point> delivery_points;
    delivery_points.reserve(count);
    for (int i = 0; i < count; ++i) {
        auto& distances_i = points[i].distances;
        std::forward_list<distance_from_location> distances_fwd_list;
        for (int j = distances_i.count - 1; j >= 0; --j) {
            auto& dist_from_point = distances_i.items[j];
            distances_fwd_list.emplace_front(dist_from_point.id, dist_from_point.distance);
        }
        delivery_points.emplace_back(points[i].id, distances_fwd_list);
    }
    SolveTspInstance(delivery_points);
    for (int i = 0; i < count; ++i) {
        ids[i] = delivery_points[i].id;
    }
}

delivery_point::operator location() const
{
    return location{id, distances};
}

void SolveTspInstance(std::vector<delivery_point>& all_delivery_points)
{
    std::vector<location> all_locations;
    all_locations.reserve(all_delivery_points.size());
    std::ranges::transform(all_delivery_points, std::back_inserter(all_locations),
        [](const auto& point){ return static_cast<location>(point); });
    GeneticTravelingSalesman tsp{all_locations};
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

    for (int i = 0; i < all_delivery_points.size(); ++i) {
        all_delivery_points[i].id = result[i].id;
        all_delivery_points[i].distances = std::move(result[i].distances);
    }
}
