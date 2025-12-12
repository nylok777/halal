//
// Created by david on 11/19/25.
//

#ifndef HALAL_GENETICTRAVELINGSALESMAN_H
#define HALAL_GENETICTRAVELINGSALESMAN_H
#include "Chromosome.h"
#include "TravelingSalesmanProblem.h"


class GeneticTravelingSalesman final : public TravelingSalesmanProblem, public Chromosome<route>
{
public:
    explicit GeneticTravelingSalesman(std::vector<location>& all_locations);
    route CrossOver(const std::vector<route>&) const override;
    void Mutate(route&) const override;
};

#endif //HALAL_GENETICTRAVELINGSALESMAN_H