//
// Created by david on 10/23/25.
//

#ifndef HALAL_SYMMETRICMATRIX_HPP
#define HALAL_SYMMETRICMATRIX_HPP

#include <vector>

template <typename T>
class SymmetricMatrix
{
public:
    explicit SymmetricMatrix(const int size) : n(size), data(size * (size + 1) / 2) {}

    T& operator()(const int i, const int j);

    const T& operator()(const int i, const int j) const;

private:
    int n;
    std::vector<T> data;

    static int Index(int i, int j);
};

template <typename T>
T& SymmetricMatrix<T>::operator()(const int i, const int j)
{
    return data[Index(i, j)];
}

template <typename T>
const T& SymmetricMatrix<T>::operator()(const int i, const int j) const
{
    return data[Index(i, j)];
}

template <typename T>
int SymmetricMatrix<T>::Index(int i, int j)
{
    if (i < j) std::swap(i, j);
    return i * (i + 1) / 2 + j;
}


#endif //HALAL_SYMMETRICMATRIX_HPP
