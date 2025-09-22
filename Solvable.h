//
// Created by david on 9/21/25.
//
#pragma once

#ifndef HALAL_SOLVABLE_H
#define HALAL_SOLVABLE_H

template <typename T>
class Solvable
{
protected:
    ~Solvable() = default;

public:
    virtual T GenerateElement() const = 0;
    virtual T GenerateNeighbour(const T&, float eps) const = 0;
    //virtual std::vector<T> AllNeighbours(const T&, const D&) = 0;
    virtual float Objective(const T&) const = 0;
    [[nodiscard]] virtual bool StopSearch() const = 0;
};


#endif //HALAL_SOLVABLE_H