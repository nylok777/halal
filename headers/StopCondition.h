//
// Created by david on 11/14/25.
//

#ifndef HALAL_STOPCONDITION_H
#define HALAL_STOPCONDITION_H

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

template <StopConditionFunctor Condition>
bool StopCondition(Condition& cond, float score)
{
    if constexpr (requires { cond(score); }) {
        return cond(score);
    } else if constexpr (requires { cond(); }) {
        return cond();
    } else {
        static_assert(false, "unsupported signature for stop condition");
    }
}

struct stop_condition_tag {};

class StopConditionMinChangeRate
{
public:
    using stop_condition_tag = ::stop_condition_tag;

    explicit StopConditionMinChangeRate(const float min_change_rate, const int max_fails);
    bool operator()(const float score);

private:
    float min_change_rate;
    int max_fails;
    float last_score = 0.0f;
    int fails_count = 0;
};

class StopConditionMaxIterations
{
public:
    using stop_condition_tag = ::stop_condition_tag;

    explicit StopConditionMaxIterations(const int max_iterations);
    bool operator()();

private:
    int max_iters;
    int iter_count = 0;
};

#endif //HALAL_STOPCONDITION_H