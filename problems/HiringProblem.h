//
// Created by david on 12/9/25.
//

#ifndef HALAL_HIRINGPROBLEM_H
#define HALAL_HIRINGPROBLEM_H
#include "../interfaces-and-templates/OptimizationProblem.h"
#include "../utils/WorkAssignment.h"
#include "interfaces-and-templates/Chromosome.h"

struct candidate_selection
{
    using NumberType = float;
    std::vector<int> candidates;
    int pareto_rank = 0;
    int dominated_by = 0;
    auto operator<(const candidate_selection& other) const -> bool;
    auto operator==(const candidate_selection& other) const -> bool;
};

class HiringProblem : public WorkAssignmentProblem, public ParetoOptimizationProblem<candidate_selection, float>, public Chromosome<candidate_selection>
{
public:
    HiringProblem() = default;
    explicit HiringProblem(const std::string& filename, const int n_people_to_hire);
    [[nodiscard]] auto GenerateInstance() const -> candidate_selection override;
    [[nodiscard]] auto GetObjectives() const -> std::vector<std::function<float(const candidate_selection&)>> override;
    [[nodiscard]] auto CrossOver(const std::vector<candidate_selection>& parents) const -> candidate_selection override;
    void Mutate(candidate_selection& child) const override;
protected:
    [[nodiscard]] auto SumSalary(const candidate_selection& selection) const -> float;
    [[nodiscard]] auto AvgQuality(const candidate_selection& selection) const -> float;
    int n_people_to_hire{};
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