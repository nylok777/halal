cmake_minimum_required(VERSION 4.0)
project(halal)

set(CMAKE_CXX_STANDARD 17)

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
        headers/Matrix.h
        headers/DisjunctiveGraph.h
        DisjunctiveGraph.cpp
        JobShopProblem.cpp
        headers/StochasticSolvable.h
        headers/GeneticQuadraticAssignment.h
        GeneticQuadraticAssignment.cpp
        StochasticQuadraticAssignment.h
        StochasticQuadraticAssignment.cpp
        headers/GeneticJobShop.h
        GeneticJobShop.cpp
)
add_executable(test_target tests/tests.cpp
        tests/main.cpp
        tests/tests.h
        headers/JobShopProblem.h
        headers/Matrix.h
        JobShopProblem.cpp
)
