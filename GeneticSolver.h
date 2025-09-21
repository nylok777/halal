//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_GENETICSOLVER_H
#define HALAL_GENETICSOLVER_H
#include <vector>

#include "GeneticSolvable.h"

template<typename T>
class GeneticSolver
{
    GeneticSolvable<T> problem;

public:
    explicit GeneticSolver(GeneticSolvable<T> problem);
    std::vector<T> InitPopulation(const int&);
    std::vector<float> Evaluation(const std::vector<T>&);
    std::vector<T> Selection(const std::vector<T>&, const std::vector<float>&, const int&);
    std::vector<T> Reinsertion(std::vector<T>&, std::vector<T>&);

    std::pair<T, float> GeneticAlgorithm(const int& k, const int& size, const int& m);
};

#endif //HALAL_GENETICSOLVER_H