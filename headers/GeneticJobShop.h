//
// Created by david on 10/5/25.
//
#pragma once

#ifndef HALAL_GENETICJOBSHOP_H
#define HALAL_GENETICJOBSHOP_H
#include "GeneticSolvable.h"
#include "JobShopProblem.h"
#include "Matrix.h"

class GeneticJobShop : public virtual GeneticSolvable<Matrix<operation>>
{
public:
    explicit GeneticJobShop(const JobShopProblem& job_shop_problem);
    std::pair<Matrix<operation>, float> GetBest(const std::vector<Matrix<operation>>&, const std::vector<float>&) override;
    Matrix<operation> Mutate(Matrix<operation>&) override;
    Matrix<operation> CrossOver(const std::vector<Matrix<operation>>&) override;
    JobShopProblem& GetProblem() override;
private:
    JobShopProblem job_shop_problem;
};

#endif //HALAL_GENETICJOBSHOP_H