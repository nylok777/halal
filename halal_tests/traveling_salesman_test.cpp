//
// Created by david on 11/25/25.
//
#include <random>
#include <ranges>
#include <cmath>
#include <gtest/gtest.h>

#include "headers/GeneticAlgorithmSolver.hpp"
#include "headers/GeneticTravelingSalesman.h"
#include "headers/location.h"

class TravelingSalesmanTest : public ::testing::Test
{
protected:
    std::string str{"str"};
};

struct test_point
{
    int id;
    int x;
    int y;
};

std::vector<location> GeneratePoints(const int n)
{
    std::vector<location> delivery_points;
    delivery_points.reserve(n);
    std::vector<test_point> test_points;
    test_points.reserve(n);
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int> int_dist{0, 100};
    for (int i = 1; i < n+1; ++i) {
        test_points.emplace_back(test_point{i, int_dist(gen), int_dist(gen)});
    }
    for (const auto & [id, x, y] : test_points) {
        std::vector<distance_from_location> distances; distances.reserve(test_points.size() - 1);
        auto other_locs = std::ranges::views::filter(test_points, [id](const auto& other)
        {
            return id != other.id;
        });
        for (const auto & [other_id, x2, y2] : other_locs) {
            auto dist = std::sqrt(std::pow(static_cast<double>(x - x2), 2) + std::pow(static_cast<double>(y - y2), 2));
            distances.emplace_back(other_id, dist);
        }
        delivery_points.emplace_back(id, distances);
    }
    return delivery_points;
}

TEST_F(TravelingSalesmanTest, GeneratePointsTest)
{
    std::vector<location> locs = GeneratePoints(5);
    ASSERT_EQ(locs.size(), 5);
    ASSERT_EQ(locs.at(0).distances.at(0).second, locs.at(1).distances.at(0).second);
}

TEST_F(TravelingSalesmanTest, GeneticAlgorithmTest)
{
    std::vector<location> locs = GeneratePoints(5);
    GeneticTravelingSalesman tsp{locs};
    auto tsp_ptr = std::make_unique<GeneticTravelingSalesman>(tsp);
    GeneticAlgorithmSolver<route, StopConditionMaxIterations> solver{std::move(tsp_ptr), StopConditionMaxIterations{100}};
    auto result = solver.GeneticAlgorithm(5, 100, 25, 50, 0.5f);
    ASSERT_EQ(result.rep.size(), 5);
    std::vector<int> ids; ids.reserve(5);
    std::ranges::transform(result.rep, std::back_inserter(ids), [](auto& x) { return x.id; });
    auto [first, last] = std::ranges::unique(ids);
    ids.erase(first, last);
    ASSERT_EQ(ids.size(), 5);
}