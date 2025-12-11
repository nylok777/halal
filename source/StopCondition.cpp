//
// Created by david on 11/14/25.
//
#include <cmath>
#include "../headers/StopCondition.h"

StopConditionMinChangeRate::StopConditionMinChangeRate(const float min_change_rate, const int max_fails)
    : min_change_rate(min_change_rate), max_fails(max_fails) {}

bool StopConditionMinChangeRate::operator()(const float score)
{
    if ((std::fabsf(score) - std::fabsf(last_score)) / std::fabsf(last_score) * 100.0f < std::fabsf(min_change_rate)) {
        ++fails_count;
    } else {
        fails_count = 0;
    }
    return fails_count < max_fails;
}

StopConditionMaxIterations::StopConditionMaxIterations(const int max_iterations)
    : max_iters(max_iterations) {}

bool StopConditionMaxIterations::operator()()
{
    ++iter_count;
    return iter_count < max_iters;
}