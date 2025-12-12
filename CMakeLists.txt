cmake_minimum_required(VERSION 3.3...4.0)
project(halal)
set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

add_library(tsp_solver SHARED
        source/TravelingSalesmanProblem.cpp
        source/GeneticTravelingSalesman.cpp
        source/StopCondition.cpp
        headers/Solution.h
        headers/GeneticAlgorithmSolver.hpp
        tsp_solver.cpp
)

set(TSP_HEADERS
        "include/tsp_solver_c.h"
        "include/tsp_solver.h"
)

set_target_properties(tsp_solver PROPERTIES
        PUBLIC_HEADER "${TSP_HEADERS}"
)

include_directories(.)
include_directories("headers")

install(TARGETS tsp_solver
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_executable(halal
        main.cpp
        source/QuadraticAssignmentProblem.cpp
        source/HiringProblem.cpp
        headers/NSGAIISolver.hpp
        source/WorkAssignment.cpp
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
        source/TravelingSalesmanProblem.cpp
        source/GeneticTravelingSalesman.cpp
        headers/NSGAIISolver.hpp
        headers/RangesExt.h
        headers/GeneticHiringProblem.h
        source/GeneticHiringProblem.cpp
)

install()
#add_subdirectory(halal_tests)