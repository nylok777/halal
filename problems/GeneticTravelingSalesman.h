//
// Created by david on 11/19/25.
//

#ifndef HALAL_GENETICTRAVELINGSALESMAN_H
#define HALAL_GENETICTRAVELINGSALESMAN_H
#include "../interfaces-and-templates/Chromosome.h"
#include "TravelingSalesmanProblem.h"


class GeneticTravelingSalesman final : public TravelingSalesmanProblem, public Chromosome<route>
{
public:
    explicit GeneticTravelingSalesman(std::vector<location>& all_locations);
    [[nodiscard]] auto CrossOver(const std::vector<route>& parents) const -> route override;
    void Mutate(route& child) const override;
};

#endif //HALAL_GENETICTRAVELINGSALESMAN_H