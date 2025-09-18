//
// Created by david on 9/17/25.
//

#include "QuadraticAssignment.h"

#include <array>
#include <random>
#include <set>

QuadraticAssignment::assignment::assignment(std::pair<int, int> fac, std::pair<int, int> loc, const vector<vector<float>>& w, const vector<vector<int>>& d)
{
    this->facilities = fac;
    this->locations = loc;
    this->weightMatrixPtr = &w;
    this->distanceMatrixPtr = &d;
}

QuadraticAssignment::QuadraticAssignment(const int n)
{
    this->n = n;
    this->maxDrought = 5;
    this->droughtRadius = 0.2;
    std::random_device rnd;
    std::mt19937 gen { rnd() };
    std::uniform_real_distribution<float> weight_dist(0.1, 10.0);
    std::uniform_int_distribution<> dist_dist(5000, 40000);

    int k = 0;
    weightMatrix = vector(n, vector<float>(n, 0.0));
    distanceMatrix = vector(n, vector<int>(n, 0.0));
    for (int i = 0; i < n; ++i)
    {
        for (int j = k; j < n; ++j)
        {
            weightMatrix[i][j] = weight_dist(gen);
            weightMatrix[j][i] = weight_dist(gen);
            distanceMatrix[i][j] = dist_dist(gen);
            distanceMatrix[j][i] = dist_dist(gen);
        }
        k++;
    }
}

QuadraticAssignment::quadratic_assignment QuadraticAssignment::GenerateAssignment() const
{
    std::random_device rnd;
    std::mt19937 gen { rnd() };
    std::uniform_int_distribution<> dist(0, this->n - 1);

    std::set<assignment*, std::equal_to<>> assigned;
    auto elemCount = 0;
    while (elemCount < this->n)
    {
        auto fac_x = dist(gen);
        auto fac_y = dist(gen);
        auto loc_x = dist(gen);
        auto loc_y = dist(gen);
        const auto ass = new assignment { std::pair(fac_x, fac_y), std::pair(loc_x, loc_y), this->weightMatrix, this->distanceMatrix };

        if (auto [iter, inserted] = assigned.insert(ass); inserted)
        {
            elemCount++;
        }
        delete ass;
    }

    return quadratic_assignment { vector<assignment>(*assigned.begin(), *assigned.end())};
}

QuadraticAssignment::quadratic_assignment QuadraticAssignment::GenerateNeighbour(const quadratic_assignment& p, const float& eps) const
{
    std::random_device rnd;
    std::mt19937 gen { rnd() };
    //std::uniform_real_distribution<float> dist(0.001, eps);
    std::uniform_int_distribution<> int_dist(0, this->n - 1);

    //float circumference = 2.0 * eps * atanf(1.0)*4.0;

    const auto idx = int_dist(gen);

    const auto& point = p.quad[idx];
    const auto& [h, k] = point.locations;
    const auto radSquare = eps*eps;
    const vector<assignment> neighbour = p.quad;
    std::array<assignment*, 2> inCircle{};
    auto count = 0;
    for (auto assignment : neighbour)
    {
        auto [x, y] = assignment.locations;
        if (const auto eq = (x - h)^2 + (y - k)^2; eq < radSquare)
        {
            inCircle[count] = &assignment;
            count++;
        }
        if (count > 1)
        {
            const auto tmp = inCircle[0]->facilities;
            inCircle[0]->facilities = inCircle[1]->facilities;
            inCircle[1]->facilities = tmp;
            count = 0;
        }
    }

    return quadratic_assignment { neighbour };
}


float QuadraticAssignment::Fitness(const quadratic_assignment& q) const
{
    float sum = 0;
    for (auto assignment : q.quad)
    {
        auto [f1, f2] = assignment.facilities;
        auto [l1, l2] = assignment.locations;
        sum += this->weightMatrix[f1][f2] * this->distanceMatrix[l1][l2];
    }

    if (sum > bestFitness)
    {
        bestFitness = sum;
    }

    return sum;
}

bool QuadraticAssignment::StopSearch() const
{
    if (fabsf(currentFitness - bestFitness) < droughtRadius)
    {
        droughtCount++;
    } else
    {
        droughtCount = 0;
    }
    if (droughtCount >= maxDrought)
    {
        return true;
    }
    return false;
}

QuadraticAssignment::quadratic_assignment QuadraticAssignment::StochasticHillClimb(const float& eps) const
{
    auto p = GenerateAssignment();
    while (!StopSearch())
    {
        if (auto q = GenerateNeighbour(p, eps); Fitness(q) < Fitness(p))
        {
            p = q;
        }
    }

    return p;
}

