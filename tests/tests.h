//
// Created by david on 10/22/25.
//

#ifndef HALAL_TESTS_H
#define HALAL_TESTS_H
#include "headers/Matrix.h"
#include "headers/JobShopProblem.h"
#include "headers/GeneticJobShop.h"

namespace JobShopTests
{
    class JobShopTesting
    {
    public:
        JobShopTesting(std::vector<operation>& instance, const int machine_num, const int job_num);
        Matrix<operation> GenerateElementTest();
        void ObjectiveTest();
        void CrossOverTest(const std::vector<Matrix<operation>>& parents);
    private:
        JobShopProblem problem;
        GeneticJobShop genetic_job_shop;
    };
}

#endif //HALAL_TESTS_H