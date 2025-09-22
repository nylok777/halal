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
        headers/Solvable.h
        headers/GeneticAlgorithmSolver.h
        headers/GeneticSolvable.h
        headers/JobShopProblem.h
        headers/Matrix.h
        headers/DisjunctiveGraph.h
        DisjunctiveGraph.cpp
        JobShopProblem.cpp
        headers/StochasticSolvable.h
)
