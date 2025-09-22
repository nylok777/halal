//
// Created by david on 9/22/25.
//

#ifndef HALAL_DISJUNCTIVEGRAPH_H
#define HALAL_DISJUNCTIVEGRAPH_H
#include <set>
#include <unordered_set>
#include <vector>

struct vertice
{
    int id;
    std::set<vertice*> directed_cons;
    std::set<vertice*> undirected_cons;
    vertice(const int id, std::set<vertice*>& d_cons, std::set<vertice*>& u_cons);
};
struct disjunctive_graph
{
    int n;
    std::set<vertice> vertices;
    explicit disjunctive_graph(std::set<vertice>& vertices);
};
#endif //HALAL_DISJUNCTIVEGRAPH_H