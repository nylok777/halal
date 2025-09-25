//
// Created by david on 9/22/25.
//
#include "headers/DisjunctiveGraph.h"

disjunctive_graph::disjunctive_graph(std::set<vertex>& vertices, std::vector<u_edge>& u_edges, std::vector<d_edge>& d_edges)
    : vertices(std::move(vertices)), u_edges(std::move(u_edges)), d_edges(std::move(d_edges)) {}
