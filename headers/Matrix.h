//
// Created by david on 9/22/25.
//
#pragma once

#ifndef HALAL_MATRIX_H
#define HALAL_MATRIX_H
#include <vector>

template<typename T>
struct matrix
{
private:
    int n;
    int m;
    std::vector<std::vector<T>> data;
public:
 //   matrix(const int n, const int m) : n(n), m(m), data(std::vector<T>(m), n) {}
    matrix(const int n, const int m, std::vector<std::vector<T>> matrix) : n(n), m(m), data(matrix) {}
    T& operator()(const int i, const int j) {
        return data[i][j];
    }

    const T& operator()(const int i, const int j) const {
        return data[i][j];
    }

    std::vector<T>& row(const int i) {
        return data[i];
    }

    const std::vector<T>& row(const int i) const {
        return data[i];
    }

    std::vector<T&> column(const int j) {
        auto c = std::vector<T&>();
        c.capacity(n);
        for (auto r : data) {
            c.push_back(r[j]);
        }
        return c;
    }

    std::vector<const T&> column(const int j) const {
        auto c = std::vector<const T&>();
        c.capacity(n);
        for (auto r : data) {
            c.push_back(r[j]);
        }
        return c;
    }
};

template <typename T>
struct symmetric_matrix
{
private:
    int n;
    std::vector<T> data;

    static int index(int i, int j) {
        if (i < j) std::swap(i, j);
        return i * (i + 1) / 2 + j;
    }

public:
    explicit symmetric_matrix(const int size) : n(size), data(size * (size + 1) / 2) {}

    T& operator()(const int i, const int j) {
        return data[index(i, j)];
    }

    const T& operator()(const int i, const int j) const {
        return data[index(i, j)];
    }
};
#endif //HALAL_MATRIX_H