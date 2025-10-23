//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_MATRIX_H
#define HALAL_MATRIX_H
#include <iostream>
#include <vector>

template <typename T>
class Matrix;

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix);

template <typename T>
class Matrix
{
public:
    Matrix() = default;

    Matrix(const int n, const int m);

    Matrix(const int n, const int m, std::vector<std::vector<T>>& data);

    T& operator()(const int i, const int j);

    T& operator()(const int i, const int j) const;

    std::vector<T> ToVector() const;

    std::vector<T>& Row(const int i);

    const std::vector<T>& Row(const int i) const;

    std::vector<T&> Column(const int j);
    std::vector<const T&> Column(const int j) const;

    int RowNum() const;

    int ColumnNum() const;

    int Size() const;

    friend std::ostream& operator<<<T>(std::ostream& stream, const Matrix<T>& matrix);

private:
    int n;
    int m;
    std::vector<std::vector<T>> data;
};

template <typename T>
Matrix<T>::Matrix(const int n, const int m) : n(n), m(m), data(std::vector(n, std::vector<T>(m, T()))) {}

template <typename T>
Matrix<T>::Matrix(const int n, const int m, std::vector<std::vector<T>>& data) : n(n), m(m), data(std::move(data)) {}

template <typename T>
T& Matrix<T>::operator()(const int i, const int j) { return data[i][j]; }

template <typename T>
T& Matrix<T>::operator()(const int i, const int j) const { return data[i][j]; }

template <typename T>
int Matrix<T>::RowNum() const { return n; }

template <typename T>
int Matrix<T>::ColumnNum() const
{
    return m;
}

template <typename T>
int Matrix<T>::Size() const
{
    return n * m;
}

template <typename T>
std::vector<T>& Matrix<T>::Row(const int i) { return data[i]; }

template <typename T>
const std::vector<T>& Matrix<T>::Row(const int i) const { return data[i]; }

template <typename T>
std::vector<T&> Matrix<T>::Column(const int j)
{
    auto c = std::vector<T&>();
    c.capacity(n);
    for (auto r : data) {
        c.push_back(r[j]);
    }
    return c;
}

template <typename T>
std::vector<const T&> Matrix<T>::Column(const int j) const
{
    auto c = std::vector<const T&>();
    c.capacity(n);
    for (auto r : data) {
        c.push_back(r[j]);
    }
    return c;
}


template <typename T>
std::vector<T> Matrix<T>::ToVector() const
{
    auto flat = std::vector<T>(n * m);
    auto from = 0;
    for (int i = 0; i < n; ++i) {
        flat.insert(data[i].cbegin() + from, data[i].cbegin(), data[i].cend());
        from += data[i].size();
    }
    return flat;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix)
{
    for (int i = 0; i < matrix.RowNum(); ++i) {
        const auto& row = matrix.Row(i);
        for (const auto& item : row) {
            stream << item << ' ';
        }
        stream << '\n';
    }
    return stream;
}

#endif //HALAL_MATRIX_H
