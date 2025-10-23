//
// Created by david on 10/22/25.
//

#include "tests.h"
using namespace JobShopTests;

int main()
{
    std::vector<operation> instance = {
        {1, 1, 1, 4}, {2, 1, 2, 6}, {3, 1, 1, 3},
        {1, 2, 2, 5}, {2, 2, 1, 2}, {3, 2, 2, 4},
        {1, 3, 1, 7}, {2, 3, 2, 3}, {3, 3, 1, 1}
    };
    JobShopTesting tester{instance, 2, 3};
    auto e = tester.GenerateElementTest();
    //std::vector<Matrix<operation>> parents;
    // for (int i = 0; i < 10; ++i) {
    //     parents.push_back(tester.GenerateElementTest());
    // }
    //std::cout << '\n' << std::endl;
    //tester.CrossOverTest(parents);
    return 0;
}