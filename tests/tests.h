//
// Created by david on 10/22/25.
//

#ifndef HALAL_TESTS_H
#define HALAL_TESTS_H
#include "headers/JobShopProblem.h"
#include "headers/GeneticJobShop.h"

namespace JobShopTests
{
    class JobShopTesting
    {
    public:
        JobShopTesting(std::vector<operation>& instance, const int machine_num, const int job_num);
        DynamicMatrix<operation> GenerateElementTest();
        void ObjectiveTest();
        void CrossOverTest(const std::vector<DynamicMatrix<operation>>& parents);

    private:
        JobShopProblem problem;
        GeneticJobShop genetic_job_shop;
    };
}

#endif //HALAL_TESTS_H