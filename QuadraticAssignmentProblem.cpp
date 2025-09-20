//
// Created by david on 9/17/25.
//

#include "QuadraticAssignmentProblem.h"
#include <random>
#include <unordered_set>

template<typename T>
float FrobeniusNorm(const vector<vector<T> > & matrix) {
    float sum = 0.0;
    for (auto row : matrix) {
        for (auto x : row) {
            sum += x * x;
        }
    }
    return std::sqrt(sum);
}

vector<vector<float> > RandomPerturbationMatrix(int n) {
    std::random_device rnd;
    std::mt19937 gen {rnd()};
    std::normal_distribution<float> dist(0.0, 1.0);

    auto matrix = vector(n, vector<float>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = dist(gen);
        }
    }

    return matrix;
}

int CantorPair(const int a, const int b) {
    return 0.5 * (a + b) * (a + b + 1) + b;
}


assignment::assignment(const std::pair<int, int> &fac, const std::pair<int, int> &loc,
                                                   const vector<vector<float> > &w, const vector<vector<int> > &d) {
    this->facilities = fac;
    this->locations = loc;
    this->weightMatrixPtr = &w;
    this->distanceMatrixPtr = &d;
}

QuadraticAssignmentProblem::QuadraticAssignmentProblem(const int n) {
    this->n = n;
    this->maxDrought = 10;
    this->droughtRadius = 0.2;
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<float> weight_dist(0.1, 10.0);
    std::uniform_int_distribution<> dist_dist(5, 40);

    int k = 0;
    weightMatrix = vector(n, vector<float>(n, 0.0));
    distanceMatrix = vector(n, vector<int>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = k; j < n; ++j) {
            weightMatrix[i][j] = weight_dist(gen);
            weightMatrix[j][i] = weight_dist(gen);
            distanceMatrix[i][j] = dist_dist(gen);
            distanceMatrix[j][i] = dist_dist(gen);
        }
        k++;
    }
}

quadratic_assignment QuadraticAssignmentProblem::GenerateAssignment() const {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> dist(0, this->n - 1);

    std::unordered_set<assignment *> assigned;
    auto elemCount = 0;
    while (elemCount < this->n) {
        auto fac_x = dist(gen);
        auto fac_y = dist(gen);
        auto loc_x = dist(gen);
        auto loc_y = dist(gen);
        const auto ass = new assignment{
            std::pair(fac_x, fac_y), std::pair(loc_x, loc_y), this->weightMatrix, this->distanceMatrix
        };

        if (assigned.find(ass) == assigned.end()) {
            assigned.insert(ass);
            elemCount++;
        } else {
            delete ass;
        }
    }
    vector<assignment> v;
    for (auto a: assigned) {
        v.push_back(*a);
    }
    return quadratic_assignment{v};
}

std::pair<int, int> RandomLocation(const quadratic_assignment &p, const int &n) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_int_distribution<> int_dist(0, n - 1);

    const auto idx = int_dist(gen);

    const auto &point = p.quad[idx];
    const auto &[h, k] = point.locations;
    return {h, k};
}

float RandomRadius(const float &eps) {
    std::random_device rnd;
    std::mt19937 gen{rnd()};
    std::uniform_real_distribution<float> dist(1.0, eps);
    return dist(gen);
}

vector<vector<int> > QuadraticAssignmentProblem::CantorPairMatrix(vector<assignment> quad) {
}


quadratic_assignment QuadraticAssignmentProblem::GenerateNeighbour(
    const quadratic_assignment &p, const float &eps) const {


    auto perturb = RandomPerturbationMatrix(p.quad.size());

}


float QuadraticAssignmentProblem::Objective(const quadratic_assignment &q) const {
    float sum = 0;
    for (auto assignment: q.quad) {
        auto [f1, f2] = assignment.facilities;
        auto [l1, l2] = assignment.locations;
        sum += this->weightMatrix[f1][f2] * this->distanceMatrix[l1][l2];
    }

    currentFitness = sum;

    if (sum < bestFitness) {
        bestFitness = sum;
    }

    return sum;
}

bool QuadraticAssignmentProblem::StopSearch() const {
    if (fabsf(currentFitness - bestFitness) < droughtRadius) {
        droughtCount++;
    } else {
        droughtCount = 0;
    }
    if (droughtCount >= maxDrought) {
        return true;
    }
    return false;
}

std::pair<quadratic_assignment, float> QuadraticAssignmentProblem::StochasticHillClimb(
    const int &eps) const {
    auto p = GenerateAssignment();
    while (!StopSearch()) {
        if (auto q = GenerateNeighbour(p, eps); Objective(q) < Objective(p)) {
            p = q;
        }
    }

    return {p, bestFitness};
}
