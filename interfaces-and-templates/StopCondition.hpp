//
// Created by david on 11/14/25.
//

#ifndef HALAL_STOPCONDITION_H
#define HALAL_STOPCONDITION_H
#include <concepts>
#include <cmath>
#include "../utils/ConceptsExt.h"


template <typename T, typename = void>
struct is_stop_condition : std::false_type {};

template <typename T>
struct is_stop_condition<T, std::void_t<typename T::stop_condition_tag>> : std::true_type {};

template <class T>
concept StopConditionFunctor =
    is_stop_condition<T>::value && requires(T cond)
    {
        { cond } -> std::same_as<T&>;
    };

template <StopConditionFunctor Condition, Numeric ScoreType>
constexpr bool StopCondition(Condition& cond, ScoreType score)
{
    if constexpr (requires { cond(score); }) {
        return cond(score);
    } else {
        return cond();
    }
}

struct stop_condition_tag {};

class StopConditionMinChangeRate
{
public:
    using stop_condition_tag = stop_condition_tag;

    explicit StopConditionMinChangeRate(const float min_change_rate, const int max_fails)
        :
        min_change_rate(min_change_rate),
        max_fails(max_fails) {}

    template<Numeric ScoreType>
    bool operator()(const ScoreType score)
    {
        const auto score_f = static_cast<float>(score);
        if ((std::fabsf(score_f) - std::fabsf(last_score_f)) /
            std::fabsf(last_score_f) * 100.0f < std::fabsf(min_change_rate)) {
            ++fails_count;
        } else {
            fails_count = 0;
        }
        last_score_f = score_f;
        return fails_count > max_fails;
    }

private:
    float min_change_rate;
    int max_fails;
    float last_score_f{};
    int fails_count = 0;
};

class StopConditionMaxIterations
{
public:
    using stop_condition_tag = ::stop_condition_tag;

    explicit StopConditionMaxIterations(const int max_iterations) : max_iters(max_iterations) {}
    bool operator()()
    {
        ++iter_count;
        return iter_count > max_iters;
    }

private:
    int max_iters;
    int iter_count = 0;
};

#endif //HALAL_STOPCONDITION_H