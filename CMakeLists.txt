cmake_minimum_required(VERSION 4.0)
project(halal)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(halal
        HillClimbSolvable.h
        QuadraticAssignmentProblem.cpp
        QuadraticAssignmentProblem.h
        main.cpp
        SimulatedAnnealingSolvable.h
        Solvable.h
)
