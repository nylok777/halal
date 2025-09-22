//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_SOLVABLE_H
#define HALAL_SOLVABLE_H

template <typename T>
class Solvable
{
public:
    virtual T GenerateElement() const = 0;
    virtual float Objective(const T&) const = 0;
    [[nodiscard]] virtual bool StopCondition() const = 0;

protected:
    ~Solvable() = default;
};


#endif //HALAL_SOLVABLE_H