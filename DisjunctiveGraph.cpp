//
// Created by david on 9/22/25.
//
#include "headers/DisjunctiveGraph.h"

vertice::vertice(const int id, std::set<vertice*>& d_cons, std::set<vertice*>& u_cons)
    : id(id), directed_cons(std::move(d_cons)), undirected_cons(std::move(u_cons)) {
}

disjunctive_graph::disjunctive_graph(std::set<vertice>& vertices)
    : n(vertices.size()), vertices(std::move(vertices)) {
}



