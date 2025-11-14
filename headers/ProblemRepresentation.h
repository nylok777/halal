//
// Created by david on 9/21/25.
//
#ifndef HALAL_PROBLEM_H
#define HALAL_PROBLEM_H

template <typename T>
class ProblemRepresentation
{
public:
    virtual ~ProblemRepresentation() = default;
    virtual T GenerateInstance() const = 0;
    virtual float Objective(const T&) const = 0;
};


#endif //HALAL_PROBLEM_H