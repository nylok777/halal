//
// Created by david on 10/22/25.
//
#include <iostream>
#include "headers/GeneticJobShop.h"
#include "headers/JobShopProblem.h"
#include "tests.h"

namespace JobShopTests
{
    JobShopTesting::JobShopTesting(std::vector<operation>& instance, const int machine_num, const int job_num)
        : problem(JobShopProblem{machine_num, job_num, instance}), genetic_job_shop(problem) {}


    DynamicMatrix<operation> JobShopTesting::GenerateElementTest()
    {
        const auto element = problem.GenerateElement();

        std::cout << element;
        return element;
    }

    void JobShopTesting::ObjectiveTest()
    {
        const auto element = problem.GenerateElement();
        const auto fitness = problem.Objective(element);
        std::cout << fitness;
    }

    void JobShopTesting::CrossOverTest(const std::vector<DynamicMatrix<operation>>& parents)
    {
        auto child = genetic_job_shop.CrossOver(parents);
        std::cout << child;
    }
}