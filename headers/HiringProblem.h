//
// Created by david on 12/9/25.
//

#ifndef HALAL_HIRINGPROBLEM_H
#define HALAL_HIRINGPROBLEM_H
#include "ProblemRepresentation.h"
#include "WorkAssignment.h"

struct candidate_selection
{
    using ObjectiveReturnType = float;
    std::vector<int> candidates;
    int pareto_rank;
    int dominated_by;
};

class HiringProblem : public WorkAssignmentProblem, public ParetoOptimizationProblem<candidate_selection, float>
{
public:
    explicit HiringProblem(const std::string& filename, const int n_people_to_hire);
    candidate_selection GenerateInstance() const override;
    bool IsParetoDominatedBy(const candidate_selection& a, const candidate_selection& b) const override;
    std::vector<std::function<float(candidate_selection&)>> GetObjectives() const override;
    std::vector<float> Objectives(candidate_selection&) const override;
protected:
    int n_people_to_hire;
};

#endif //HALAL_HIRINGPROBLEM_H