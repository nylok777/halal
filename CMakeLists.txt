cmake_minimum_required(VERSION 3.3...4.0)
project(halal)
set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

add_library(tsp_solver SHARED
        problems/TravelingSalesmanProblem.cpp
        problems/GeneticTravelingSalesman.cpp
        interfaces-and-templates/Solution.h
        solvers/GeneticAlgorithm.hpp
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
include_directories("utils")

install(TARGETS tsp_solver
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_executable(halal
        main.cpp
        problems/QuadraticAssignmentProblem.cpp
        problems/HiringProblem.cpp
        solvers/NSGAII.hpp
        utils/WorkAssignment.cpp
        solvers/GeneticAlgorithm.hpp
        problems/JobShopProblem.cpp
        problems/GeneticQuadraticAssignment.cpp
        problems/GeneticJobShop.cpp
        utils/DynamicMatrix.hpp
        utils/SymmetricMatrix.hpp
        interfaces-and-templates/Solution.h
        problems/StochasticQuadraticAssignment.cpp
        solvers/SimulatedAnnealing.hpp
        interfaces-and-templates/StopCondition.hpp
        problems/TravelingSalesmanProblem.cpp
        problems/GeneticTravelingSalesman.cpp
        solvers/NSGAII.hpp
        utils/RangesExt.h
        problems/GeneticHiringProblem.h
        problems/GeneticHiringProblem.cpp
        solvers/HillClimb.hpp
        utils/ConceptsExt.h
)

install()
#add_subdirectory(halal_tests)