//
// Created by david on 12/11/25.
//

#ifndef HALAL_GENETICHIRINGPROBLEM_H
#define HALAL_GENETICHIRINGPROBLEM_H
#include "../interfaces-and-templates/Chromosome.h"
#include "HiringProblem.h"

class GeneticHiringProblem : public HiringProblem, public Chromosome<HiringProblem::SolutionType>
{
public:
    GeneticHiringProblem(const std::string& filename, const int n_people_to_hire);
    [[nodiscard]] auto CrossOver(const std::vector<candidate_selection>& parents) const -> candidate_selection override;
    void Mutate(candidate_selection& child) const override;
};

#endif //HALAL_GENETICHIRINGPROBLEM_H