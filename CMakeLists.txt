cmake_minimum_required(VERSION 4.0)
project(halal)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
add_executable(halal
        source/QuadraticAssignmentProblem.cpp
        main.cpp
        headers/GeneticAlgorithmSolver.hpp
        source/JobShopProblem.cpp
        source/GeneticQuadraticAssignment.cpp
        source/GeneticJobShop.cpp
        headers/DynamicMatrix.hpp
        headers/SymmetricMatrix.hpp
        headers/Solution.h
        source/StochasticQuadraticAssignment.cpp
        headers/SimulatedAnnealingSolver.hpp
        source/StopCondition.cpp
        headers/StopCondition.h
)
#add_subdirectory(halal_tests)