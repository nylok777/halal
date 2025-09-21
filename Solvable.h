//
// Created by david on 9/21/25.
//


#ifndef HALAL_STOCHASTICSOLVABLE_H
#define HALAL_STOCHASTICSOLVABLE_H

template<typename T, typename D>
class Solvable {
protected:
    ~Solvable() = default;

public:
    virtual T GenerateElement() const = 0;
    virtual T GenerateNeighbour(const T&, const D&) const = 0;
    //virtual std::vector<T> AllNeighbours(const T&, const D&) = 0;
    virtual float Objective(const T&) const = 0;
    [[nodiscard]] virtual bool StopSearch() const = 0;
};


#endif //HALAL_STOCHASTICSOLVABLE_H