//
// Created by david on 10/24/25.
//
#include "gtest/gtest.h"
#include "DynamicMatrix.hpp"

class DynamicMatrixTest : public ::testing::Test
{
protected:
    DynamicMatrixTest() : matrix(DynamicMatrix<int>{2})
    {
        matrix.PushBack(0, 1);
        matrix.PushBack(0, 3);
        matrix.PushBack(0, 5);
        matrix.PushBack(1, 2);
        matrix.PushBack(1, 4);
    }

    DynamicMatrix<int> matrix;
};


TEST_F(DynamicMatrixTest, IteratorTest)
{
    int elem1 = matrix.Row(0).at(0);
    int elem_from_begin = *matrix.begin();
    ASSERT_EQ(1, elem1);
    ASSERT_EQ(elem1, elem_from_begin);
    auto iter = matrix.begin();
    ++iter;
    int elem2 = *iter;
    ASSERT_EQ(3, elem2);
    int elem3 = *(++iter);
    ASSERT_EQ(5, elem3);
    auto row2_begin = ++iter;
    ASSERT_EQ(2, *row2_begin);
    ++row2_begin;
    ++row2_begin;
    ASSERT_EQ(row2_begin, matrix.end());
}

TEST_F(DynamicMatrixTest, InsertTest)
{
    matrix.Insert(1, 0, 6);
    ASSERT_EQ(matrix.At(1, 0), 6);
}
