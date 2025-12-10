//
// Created by david on 12/10/25.
//
#include "HiringProblem.h"
#include "RangesExt.h"

#include <algorithm>
#include <random>
#include <ranges>

HiringProblem::HiringProblem(const std::string& filename, const int n_people_to_hire)
    : WorkAssignmentProblem(), n_people_to_hire(n_people_to_hire)
{
    WorkAssignmentProblem::loadFromFile(filename);
}

candidate_selection HiringProblem::GenerateInstance() const
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<float> dist;
    std::vector<int> people; people.reserve(persons.size());
    for (int i = 0; i < persons.size(); ++i) {
        if (dist(gen) < 0.5f) {
            people.push_back(i);
        }
        if (people.size() == n_people_to_hire) break;
    }
    return candidate_selection{std::move(people)};
}

bool HiringProblem::IsParetoDominatedBy(const candidate_selection& a, const candidate_selection& b) const
{
    return !(avgQuality(a.candidates) >= avgQuality(b.candidates) ||
        sumSalary(a.candidates) <= sumSalary(b.candidates));
}

std::vector<float> HiringProblem::Objectives(candidate_selection& selection) const
{
    std::vector<float> scores;
    scores.push_back(avgQuality(selection.candidates));
    scores.push_back(sumSalary(selection.candidates));
    return scores;
}

std::vector<std::function<float(candidate_selection&)>> HiringProblem::GetObjectives() const
{
    std::function<float(candidate_selection&)> sum_salary{[this](const candidate_selection& x)
    {
        return sumSalary(x.candidates);
    }};
    std::function<float(candidate_selection&)> avg_quality{[this](const candidate_selection& x)
    {
        return avgQuality(x.candidates);
    }};
    return {std::move(sum_salary), std::move(avg_quality)};
}
