//
// Created by david on 1/3/26.
//

#include "PolygonProblem.h"

#include <random>

#include "Matrix.hpp"

PolygonProblem::PolygonProblem(const std::string& filename)
{
    loadPointsFromFile(filename);
}

auto PolygonProblem::GenerateSolution() const -> polygon
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<float> dist;
    std::vector<Point> polygon_points(points.size());
    do {
        for (size_t i = 0; i < points.size(); ++i) {
            polygon_points.at(i) = {dist(gen), dist(gen)};
        }
    } while (constraint(polygon_points) >= 0);
    const auto score = Objective(polygon_points);
    return {std::move(polygon_points), score};
}

auto PolygonProblem::Objective(const Genotype& solution) const -> NumberType
{
    return objective(solution);
}

auto PolygonProblem::GetDirectNeighbour(const polygon& elem, float eps) const -> polygon
{
    constexpr size_t directions = 4;
    Matrix<Point> neighbours{directions, elem.genotype};
    for (size_t i = 0; i < directions; ++i) {
        switch (i) {
        case 0:
            for (auto & point : neighbours.row(i)) {
                point.x += eps;
            }
            break;
        case 1:
            for (auto & point : neighbours.row(i)) {
                point.y += eps;
            }
            break;
        case 2:
            for (auto & point : neighbours.row(i)) {
                point.x -= eps;
            }
            break;
        case 3:
            for (auto & point : neighbours.row(i)) {
                point.y -= eps;
            }
        }
    }
    auto best = neighbours.row(0);
    auto fitness = objective(static_cast<std::vector<Point>>(best));
    for (size_t i = 1; i < directions; ++i) {
        auto& q = neighbours.row(i);
        auto q_fit = objective(static_cast<std::vector<Point>>(q));
        if (q_fit < fitness) {
            best = q;
            fitness = q_fit;
        }
    }
    return polygon{static_cast<std::vector<Point>>(best), fitness};
}
