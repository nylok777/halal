//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_MATRIX_H
#define HALAL_MATRIX_H
#include <iostream>
#include <vector>

template<typename T>
class Matrix;

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix);

template <typename T>
class Matrix
{

public:
    Matrix() = default;

    Matrix(const int n, const int m) : n(n), m(m), data(std::vector(n, std::vector<T>(m, T()))) {}

    T& operator()(const int i, const int j) {return data[i][j];}

    const T& operator()(const int i, const int j) const {return data[i][j];}

    std::vector<T> to_vector() const
    {
        auto flat = std::vector<T>(n * m);
        auto from = 0;
        for (int i = 0; i < n; ++i) {
            flat.insert(data[i].cbegin() + from, data[i].cbegin(), data[i].cend());
            from += data[i].size();
        }
        return flat;
    }

    std::vector<T>& row(const int i) {return data[i];}

    const std::vector<T>& row(const int i) const {return data[i];}

    std::vector<T&> column(const int j)
    {
        auto c = std::vector<T&>();
        c.capacity(n);
        for (auto r : data) {
            c.push_back(r[j]);
        }
        return c;
    }

    std::vector<const T&> column(const int j) const
    {
        auto c = std::vector<const T&>();
        c.capacity(n);
        for (auto r : data) {
            c.push_back(r[j]);
        }
        return c;
    }

    int row_num() const {return n;}

    int column_num() const {return m;}

    int size() const {return n * m;}

    friend std::ostream& operator<<<T>(std::ostream& stream, const Matrix<T>& matrix);
private:
    int n;
    int m;
    std::vector<std::vector<T>> data;
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix)
{
    for (int i = 0; i < matrix.row_num(); ++i) {
        const auto& row = matrix.row(i);
        for (const auto & item : row) {
            stream << item << ' ';
        }
        stream << '\n';
    }
    return stream;
}
template <typename T>
struct symmetric_matrix
{
private:
    int n;
    std::vector<T> data;

    static int index(int i, int j)
    {
        if (i < j) std::swap(i, j);
        return i * (i + 1) / 2 + j;
    }

public:
    explicit symmetric_matrix(const int size) : n(size), data(size * (size + 1) / 2) {}

    T& operator()(const int i, const int j)
    {
        return data[index(i, j)];
    }

    const T& operator()(const int i, const int j) const
    {
        return data[index(i, j)];
    }
};
#endif //HALAL_MATRIX_H
