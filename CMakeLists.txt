cmake_minimum_required(VERSION 4.0)
project(halal)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
add_executable(halal
        QuadraticAssignmentProblem.cpp
        main.cpp
        headers/GeneticAlgorithmSolver.hpp
        data_structures/Matrix.hpp
        JobShopProblem.cpp
        GeneticQuadraticAssignment.cpp
        StochasticQuadraticAssignment.cpp
        GeneticJobShop.cpp
        data_structures/DynamicMatrix.hpp
        data_structures/SymmetricMatrix.hpp
        headers/SimulatedAnnealingSolver.hpp
)
#add_subdirectory(halal_tests)