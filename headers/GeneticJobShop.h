//
// Created by david on 10/5/25.
//

#ifndef HALAL_GENETICJOBSHOP_H
#define HALAL_GENETICJOBSHOP_H
#include "GeneticSolvable.h"
#include "JobShopProblem.h"

class GeneticJobShop final : public GeneticSolvable<jobshop_schedule>
{
public:
    explicit GeneticJobShop(JobShopProblem job_shop_problem);

    void Mutate(jobshop_schedule&) override;

    jobshop_schedule CrossOver(const std::vector<jobshop_schedule>&) override;

    jobshop_schedule GetBest(const std::vector<jobshop_schedule>&) override;

    jobshop_schedule GenerateInstance() override;

private:
    JobShopProblem job_shop_problem;
};

#endif //HALAL_GENETICJOBSHOP_H