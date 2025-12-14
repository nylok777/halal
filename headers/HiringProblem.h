//
// Created by david on 12/9/25.
//

#ifndef HALAL_HIRINGPROBLEM_H
#define HALAL_HIRINGPROBLEM_H
#include "OptimizationProblem.h"
#include "WorkAssignment.h"

struct candidate_selection
{
    using ObjectiveReturnType = float;
    std::vector<int> candidates{};
    int pareto_rank = 0;
    int dominated_by = 0;
    auto operator<(const candidate_selection& other) const -> bool;
    auto operator==(const candidate_selection& other) const -> bool;
};

class HiringProblem : public WorkAssignmentProblem, public ParetoOptimizationProblem<candidate_selection, float>
{
public:
    HiringProblem() = default;
    explicit HiringProblem(const std::string& filename, const int n_people_to_hire);
    [[nodiscard]] auto GenerateInstance() const -> candidate_selection override;
    [[nodiscard]] auto GetObjectives() const -> std::vector<std::function<float(const candidate_selection&)>> override;
protected:
    auto SumSalary(const candidate_selection& selection) const -> float;
    auto AvgQuality(const candidate_selection& selection) const -> float;
    int n_people_to_hire;
};

class ParetoDominanceComparator : public IParetoDominanceComparator<candidate_selection>
{
public:
    explicit ParetoDominanceComparator(const HiringProblem* problem);
    auto operator()(const candidate_selection& a, const candidate_selection& b) const -> bool override;

private:
    const HiringProblem* problem;
};

#endif //HALAL_HIRINGPROBLEM_H