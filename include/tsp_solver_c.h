//
// Created by david on 11/21/25.
//

#ifndef HALAL_TSP_SOLVER_C_H
#define HALAL_TSP_SOLVER_C_H

#ifdef __cplusplus
extern "C" {
#endif

struct distance_from_point
{
    int id;
    double distance;
};
typedef struct distance_from_point distance_from_point;

struct distances_list
{
    int count;
    distance_from_point* items;
};
typedef struct distances_list distances_list;

struct point
{
    int id;
    distances_list distances;
};
typedef struct point point;

void SolveTspInstance_C(point* points, const int count, int* ids);

#ifdef __cplusplus
    }
#endif


#endif //HALAL_TSP_SOLVER_C_H
