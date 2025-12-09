//
// Created by david on 11/10/25.
//
#include <gtest/gtest.h>

#include "headers/JobShopProblem.h"


class JobshopTest : public ::testing::Test
{
protected:
    std::string file_path{"la02.txt"};
};


TEST_F(JobshopTest, LoadFromFileTest)
{
    const auto jobshop = JobShopProblem::LoadFromFile(file_path);
    ASSERT_EQ(jobshop.NumberOfJobs(), 10);
    ASSERT_EQ(jobshop.NumberOfMachines(), 5);
    ASSERT_EQ(jobshop.NumberOfOperations(), 50);
    ASSERT_EQ(jobshop.GetOperations()[1].time, 87);
    ASSERT_EQ(jobshop.GetOperations()[0].machine_id, 1);
    ASSERT_EQ(jobshop.GetOperations()[4].machine_id, 3);
}