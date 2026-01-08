//
// Created by david on 12/10/25.
//
#include "HiringProblem.h"
#include "../interfaces-and-templates/Chromosome.h"

#include <algorithm>
#include <random>

auto candidate_selection::operator<(const candidate_selection& other) const -> bool
{
    return candidates < other.candidates;
}

auto candidate_selection::operator==(const candidate_selection& other) const -> bool
{
    return !(*this < other) && !(other < *this);
}

HiringProblem::HiringProblem(const std::string& filename, const int n_people_to_hire)
    : WorkAssignmentProblem(), n_people_to_hire(n_people_to_hire)
{
    WorkAssignmentProblem::loadFromFile(filename);
}

auto HiringProblem::GenerateSolution() const -> candidate_selection
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<float> dist;
    std::uniform_int_distribution<int> n_people{1, static_cast<int>(persons.size())};
    std::vector<int> people;
    const auto n_hired = n_people(gen);
    while (people.empty()) {
        for (int i = 0; i < persons.size(); ++i) {
            if (dist(gen) < 0.5f) {
                people.push_back(i);
            }
            if (people.size() == n_hired) break;
        }
    }
    people.shrink_to_fit();
    return candidate_selection{std::move(people)};
}

auto HiringProblem::CrossOver(const std::vector<candidate_selection>& parents) const -> candidate_selection
{
    std::mt19937 gen{std::random_device{}()};
    std::vector<int> new_candidates;
    while (new_candidates.empty()) {
        for (const auto & parent : parents) {
            std::uniform_int_distribution<size_t> dist{0, parent.candidates.size() - 1};
            auto chosen = parent.candidates.at(dist(gen));
            if (std::ranges::find(new_candidates, chosen) == std::ranges::end(new_candidates)) {
                new_candidates.emplace_back(chosen);
            }
        }
    }
    return candidate_selection{std::move(new_candidates)};
}

void HiringProblem::Mutate(candidate_selection& child) const
{
    auto [min, max] = std::ranges::minmax_element(child.candidates, std::less<>());
    ++min; --max;
}

auto HiringProblem::SumSalary(const candidate_selection& selection) const -> float
{
    return sumSalary(selection.candidates);
}

auto HiringProblem::AvgQuality(const candidate_selection& selection) const -> float
{
    return avgQuality(selection.candidates);
}

auto HiringProblem::GetObjectives() const -> std::vector<std::function<float(const candidate_selection&)>>
{
    using std::placeholders::_1;
    std::function<float(const candidate_selection&)> sum_salary = [this]<typename T0>(T0 && PH1) -> auto
    {
        return SumSalary(std::forward<T0>(PH1));
    };
    std::function<float(const candidate_selection&)> avg_quality = [this]<typename T0>(T0 && PH1) -> auto
    {
        return AvgQuality(std::forward<T0>(PH1));
    };
    return {std::move(sum_salary), std::move(avg_quality)};
}

ParetoDominanceComparator::ParetoDominanceComparator(const HiringProblem* problem)
    : problem(problem) {}

auto ParetoDominanceComparator::operator()(const candidate_selection& a, const candidate_selection& b) const -> bool
{
    const auto funcs = problem->GetObjectives();
    return funcs.at(0)(a) > funcs.at(0)(b) && funcs.at(1)(a) < funcs.at(1)(b);
}