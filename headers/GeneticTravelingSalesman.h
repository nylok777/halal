//
// Created by david on 11/19/25.
//

#ifndef HALAL_GENETICTRAVELINGSALESMAN_H
#define HALAL_GENETICTRAVELINGSALESMAN_H
#include "GeneticSolvable.h"
#include "TravelingSalesmanProblem.h"

struct route
{
    std::vector<location> rep;
    double score;
    bool operator<(const route& other) const { return score < other.score; }
};

class GeneticTravelingSalesman final : public GeneticSolvable<route>
{
public:
    explicit GeneticTravelingSalesman(std::vector<location>& all_locations);
    route CrossOver(const std::vector<route>&) override;
    void Mutate(route&) override;
    route GetBest(const std::vector<route>&) override;
    route GenerateInstance() override;
private:
    TravelingSalesmanProblem tsp;
};

#endif //HALAL_GENETICTRAVELINGSALESMAN_H