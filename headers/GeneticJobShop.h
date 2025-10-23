//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICJOBSHOP_H
#define HALAL_GENETICJOBSHOP_H
#include "GeneticSolvable.h"
#include "JobShopProblem.h"
#include "data_structures/DynamicMatrix.hpp"

class GeneticJobShop : public GeneticSolvable<DynamicMatrix<operation>>
{
public:
    explicit GeneticJobShop(JobShopProblem job_shop_problem);
    std::pair<DynamicMatrix<operation>, float>
    GetBest(const std::vector<DynamicMatrix<operation>>&, const std::vector<float>&) override;
    DynamicMatrix<operation> Mutate(DynamicMatrix<operation>&) override;
    DynamicMatrix<operation> CrossOver(const std::vector<DynamicMatrix<operation>>&) override;
    JobShopProblem& GetProblem() override;

private:
    JobShopProblem job_shop_problem;
};

#endif //HALAL_GENETICJOBSHOP_H