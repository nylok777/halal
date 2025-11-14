//
// Created by david on 11/10/25.
//
#include "gtest/gtest.h"
#include "headers/GeneticJobShop.h"


class GeneticJobshopTest : public ::testing::Test
{
protected:
    GeneticJobshopTest() : genetic_job_shop(JobShopProblem::LoadFromFile("la02.txt")) {}
    GeneticJobShop genetic_job_shop;
};


TEST_F(GeneticJobshopTest, CrossOverTest)
{
    auto parent1 = genetic_job_shop.GenerateInstance();
    auto parent2 = genetic_job_shop.GenerateInstance();
    std::vector parents{std::move(parent1), std::move(parent2)};
    auto child = genetic_job_shop.CrossOver(parents);
    ASSERT_EQ(child.rep.RowCount(), 5);
    ASSERT_EQ(child.rep.Size(), 50);
    ASSERT_EQ(child.rep.SizeOfRow(0), 10);
    auto row_is_correct = [&child](const int row_idx)
    {
        return std::ranges::all_of(
            child.rep.Row(row_idx),
            [row_idx](auto& op) { return op.machine_id == row_idx + 1; });
    };

    ASSERT_EQ(row_is_correct(0), true);
    ASSERT_EQ(row_is_correct(1), true);
    ASSERT_EQ(row_is_correct(2), true);
    ASSERT_EQ(row_is_correct(3), true);
    ASSERT_EQ(row_is_correct(4), true);
}
