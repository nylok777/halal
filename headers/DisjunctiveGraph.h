//
// Created by david on 9/22/25.
//

#ifndef HALAL_DISJUNCTIVEGRAPH_H
#define HALAL_DISJUNCTIVEGRAPH_H
#include <set>
#include <vector>

struct vertex;

struct u_edge
{
    const vertex* v1;
    const vertex* v2;
};

struct d_edge
{
    const vertex* from;
    const vertex* to;
};

struct vertex
{
    int id;
    std::set<const vertex*> u_neighbours;
    std::set<const vertex*> d_neighbours;
    friend bool operator<(const vertex& item1, const vertex& item2) {
        return item1.id < item2.id;
    }
};
struct disjunctive_graph
{
    explicit disjunctive_graph(std::set<vertex>& vertices, std::vector<u_edge>& u_edges, std::vector<d_edge>& d_edges);
    std::set<vertex> vertices;
    std::vector<u_edge> u_edges;
    std::vector<d_edge> d_edges;
};
#endif //HALAL_DISJUNCTIVEGRAPH_H