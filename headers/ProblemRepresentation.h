//
// Created by david on 9/21/25.
//
#ifndef HALAL_PROBLEM_H
#define HALAL_PROBLEM_H

template <typename T, typename Number>
class ProblemRepresentation
{
public:
    using NumberType = Number;
    virtual ~ProblemRepresentation() = default;
    virtual T GenerateInstance() const = 0;
    virtual Number Objective(const T&) const = 0;
};


#endif //HALAL_PROBLEM_H