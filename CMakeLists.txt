cmake_minimum_required(VERSION 4.0)
project(halal)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(halal
        headers/HillClimb.h
        QuadraticAssignmentProblem.cpp
        headers/QuadraticAssignmentProblem.h
        main.cpp
        headers/SimulatedAnnealingSolver.h
        headers/Problem.h
        headers/GeneticAlgorithmSolver.h
        headers/GeneticSolvable.h
        headers/JobShopProblem.h
        data_structures/Matrix.hpp
        JobShopProblem.cpp
        headers/StochasticSolvable.h
        headers/GeneticQuadraticAssignment.h
        GeneticQuadraticAssignment.cpp
        StochasticQuadraticAssignment.h
        StochasticQuadraticAssignment.cpp
        headers/GeneticJobShop.h
        GeneticJobShop.cpp
        data_structures/DynamicMatrix.hpp
        data_structures/SymmetricMatrix.hpp
)
add_subdirectory(halal_tests)