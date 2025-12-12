//
// Created by david on 10/5/25.
//

#ifndef HALAL_GENETICJOBSHOP_H
#define HALAL_GENETICJOBSHOP_H
#include "Chromosome.h"
#include "JobShopProblem.h"

class GeneticJobShop final : public JobShopProblem, public Chromosome<JobShopProblem::SolutionType>
{
public:
    explicit GeneticJobShop(int machines_num, int jobs_num, std::vector<operation> operations);

    explicit GeneticJobShop(JobShopProblem&&);

    void Mutate(jobshop_schedule&) const override;

    jobshop_schedule CrossOver(const std::vector<jobshop_schedule>&) const override;
};

#endif //HALAL_GENETICJOBSHOP_H