//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICJOBSHOP_H
#define HALAL_GENETICJOBSHOP_H
#include "GeneticSolvable.h"
#include "JobShopProblem.h"
#include "Matrix.h"

class GeneticJobShop : GeneticSolvable<matrix<operation>>
{
public:
    virtual ~GeneticJobShop() = default;
    explicit GeneticJobShop(const JobShopProblem& job_shop_problem);
    std::pair<matrix<operation>, float> GetBest(const std::vector<matrix<operation>>&, const std::vector<float>&) override;
    matrix<operation> Mutate(matrix<operation>&) override;
    matrix<operation> CrossOver(const std::vector<matrix<operation>>&) override;
    JobShopProblem& GetProblem() override;
private:
    JobShopProblem job_shop_problem;
};

#endif //HALAL_GENETICJOBSHOP_H