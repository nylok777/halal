//
// Created by david on 1/6/26.
//

#ifndef HALAL_PARTICLESWARMOPTIMIZATION_HPP
#define HALAL_PARTICLESWARMOPTIMIZATION_HPP
#include <forward_list>
#include <memory>
#include <random>

#include "StopCondition.hpp"
#include "OptimizationProblem.h"

template<typename T>
concept Pointer = std::is_pointer_v<T>;

template<typename T>
concept RealIterator = std::forward_iterator<T> && Numeric<typename T::value_type>;

template<typename T>
concept RealVector = RealIterator<typename T::iterator>;


template<std::floating_point Float>
class ParticleSwarm
{
public:
    ParticleSwarm(const Float local_weight, const Float global_weight, const Float omega)
        :
        local_w(local_weight),
        global_w(global_weight),
        omega(omega) {}

    template<SingleObjectiveProblem P, StopConditionFunctor S, std::integral Int, Solution T = P::SolutionType,
        typename V = std::vector<Float>>
    requires RealVector<typename P::Genotype>
    T operator()(const P& solvable,
        S& stop_condition,
        const Int population_size)
    {
        std::forward_list<SwarmMember<T,V>> population;
        std::unique_ptr<T> g_opt = nullptr;
        for (Int i = 0; i < population_size; ++i) {
            auto p = solvable.GenerateSolution();
            population.push_back(SwarmMember<T,V>{p, p});
            if (g_opt == nullptr || p.score < g_opt->score) {
                g_opt = std::make_unique<T>(std::move(p));
            }
        }
        while (StopCondition(stop_condition, g_opt->score)) {
            for (auto& p : population) {
                CalculateVelocity(p, g_opt.get());
                p.position += p.velocity;
                p.position.score = solvable.Objective(p.position.genotype);
            }
            for (auto& p : population) {
                if (p.position.score < p.local_opt.score) {
                    p.local_opt = p.position;
                    if (p.position.score < g_opt->score) {
                        g_opt = std::make_unique<T>(p.position);
                    }
                }
            }
        }
        return *g_opt;
    }

private:
    Float local_w;
    Float global_w;
    Float omega;
    std::uniform_real_distribution<Float> rnd;
    std::mt19937 gen{std::random_device{}()};

    template<Solution T, typename V>
    struct SwarmMember
    {
        using MemberType = T;
        T position;
        T local_opt;
        V velocity;
    };
    template<typename T, typename V>
    void CalculateVelocity(SwarmMember<T,V>& member, SwarmMember<T,V>::MemberType* g_opt)
    {
        Float rp = rnd(gen);
        Float rg = rnd(gen);
        member.velocity = omega*member.velocity + rp*local_w*(member.local_opt - member.position) +
            rg*global_w*(*g_opt - member.position);
    }
};


#endif //HALAL_PARTICLESWARMOPTIMIZATION_HPP