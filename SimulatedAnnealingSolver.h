//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_SIMULATEDANNEALINGSOLVER_H
#define HALAL_SIMULATEDANNEALINGSOLVER_H
#include <iostream>
#include <random>
#include "Solvable.h"

template<typename T>
class SimulatedAnnealingSolver {

    Solvable<T> problem;
    float temp0;
public:
    SimulatedAnnealingSolver(Solvable<T>& problem, float temp0);
    float BoltzmannScheduleTemperature(int) const;
    std::pair<T, float> SimulatedAnnealing(float eps, float kB);
};

#endif //HALAL_SIMULATEDANNEALINGSOLVER_H