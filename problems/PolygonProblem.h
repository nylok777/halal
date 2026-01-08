//
// Created by david on 1/3/26.
//

#ifndef HALAL_POLYGONPROBLEM_H
#define HALAL_POLYGONPROBLEM_H
#include "OptimizationProblem.h"
#include "SmallestBoundaryPolygon.h"
#include "Solution.h"

struct polygon : SolutionCandidate<std::vector<Point>, float> {};

class PolygonProblem
    :
    public SmallestBoundaryPolygonProblem, public OptimizationProblem<polygon>, public IDirectNeighbour<polygon>
{
public:
    explicit PolygonProblem(const std::string& filename);
    [[nodiscard]] auto GenerateSolution() const -> polygon override;
    [[nodiscard]] auto Objective(const Genotype& solution) const -> NumberType override;
    [[nodiscard]] auto GetDirectNeighbour(const polygon& elem, float eps) const -> polygon override;
};


#endif //HALAL_POLYGONPROBLEM_H