//
// Created by david on 1/3/26.
//

#ifndef HALAL_TABOOSEARCH_HPP
#define HALAL_TABOOSEARCH_HPP

#include "OptimizationProblem.h"
#include "StopCondition.hpp"

template<typename C, typename T = std::ranges::range_value_t<C>>
class TabooList
{
public:
    explicit TabooList(const size_t max_elements) : taboos(C{}), max_elements(max_elements) {}

    void SetTabooBarrier()
    {
        taboo_view = std::views::all(taboos);
    }

    bool IsTaboo(const T& t)
    {
        return std::ranges::any_of(taboo_view, [&t](const auto& x) { return x == t; });
    }

    void AddTaboo(const T& t)
    {
        if constexpr (requires (C& c, T& elem) { c.push_back(elem); }) {
            taboos.push_back(t);
        } else {
            taboos.insert(taboos.end(), t);
        }
    }

    void PurgeTaboo()
    {
        if (std::ranges::size(taboos) >= max_elements) {
            std::ranges::remove(taboos, *taboos.begin());
        }
    }

private:
    C taboos;
    std::views::all_t<C> taboo_view;
    size_t max_elements;
};

template<IsSingleObjectiveProblem P, StopConditionFunctor S, typename C, typename T = P::SolutionType>
requires Solution<T> && std::is_base_of_v<IDirectNeighbour<T>, P>
T TabooSearch(
    const P* const solvable,
    S& stop_condition,
    const TabooList<C>* taboo_list,
    const float epsilon)
{
    auto p_opt = solvable->GenerateInstance();
    while (!StopCondition(stop_condition, p_opt.score)) {
        auto p = solvable->GenerateInstance();
        bool stuck = false;
        while (!StopCondition(stop_condition, p.score) && !taboo_list->IsTaboo(p) && !stuck) {
            if (p.score < p_opt.score) {
                p_opt = p;
            }
            taboo_list->AddTaboo(p);
            taboo_list->PurgeTaboo();
            auto q = solvable->GetDirectNeighbour(p, epsilon);
            if (q.score < p.score) {
                p = q;
            } else {
                stuck = true;
            }
        }
    }
    return p_opt;
}

#endif //HALAL_TABOOSEARCH_HPP