//
// Created by david on 10/23/25.
//

#ifndef HALAL_DYNAMICMATRIX_HPP
#define HALAL_DYNAMICMATRIX_HPP
#include "data_structures/Matrix.hpp"
#include <algorithm>
#include <vector>
#include <map>

template <typename T>
class DynamicMatrix;

template <typename T>
std::ostream& operator<<(std::ostream& stream, const DynamicMatrix<T>& matrix);

template <typename T>
class DynamicMatrix
{
public:
    explicit DynamicMatrix(const int n_rows);

    void PushBack(const int n_row, const T& value);
    void PopBack(const int n_row);

    std::vector<T>& Row(const int n_row);
    const std::vector<T>& Row(const int n_row) const;

    std::vector<T> Column(const int n_col);

    int RowNum() const;

    int ColNum() const;

    int Size() const;

    int SizeOfRow(const int n_row) const;

    Matrix<T> ToFixedMatrix();

    friend std::ostream& operator<<<T>(std::ostream& stream, const DynamicMatrix<T>& matrix);

private:
    std::map<int, std::vector<T>> rows;
    int n_rows;
};

template <typename T>
DynamicMatrix<T>::DynamicMatrix(const int n_rows) : n_rows(n_rows)
{
    std::map<int, std::vector<T>> matrix_map;
    for (int i = 0; i < n_rows; ++i) {
        std::vector<T> row;
        matrix_map.insert(std::make_pair(i, row));
    }
    rows = matrix_map;
}

template <typename T>
void DynamicMatrix<T>::PushBack(const int n_row, const T& value)
{
    rows.find(n_row)->second.push_back(value);
}


template <typename T>
void DynamicMatrix<T>::PopBack(const int n_row)
{
    rows.extract(n_row).mapped().pop_back();
}

template <typename T>
std::vector<T>& DynamicMatrix<T>::Row(const int n_row)
{
    return rows.find(n_row)->second;
}

template <typename T>
const std::vector<T>& DynamicMatrix<T>::Row(const int n_row) const
{
    return rows.find(n_row)->second;
}

template <typename T>
std::vector<T> DynamicMatrix<T>::Column(const int n_col)
{
    std::vector<T> col;
    for (const auto& row : rows) {
        if (n_col >= row.second.size()) continue;
        col.push_back(row.second.at(n_col));
    }
    return col;
}

template <typename T>
int DynamicMatrix<T>::RowNum() const
{
    return n_rows;
}

template <typename T>
int DynamicMatrix<T>::ColNum() const
{
    auto largest = std::ranges::max_element(rows, [](const auto& a, const auto& b)
    {
        return a.second.size() < b.second.size();
    });
    return largest->second.size();
}


template <typename T>
int DynamicMatrix<T>::Size() const
{
    auto size = 0;
    for (const auto& row : rows) {
        size += row.second.size();
    }
    return size;
}

template <typename T>
int DynamicMatrix<T>::SizeOfRow(const int n_row) const
{
    return rows.find(n_row)->second.size();
}

template <typename T>
Matrix<T> DynamicMatrix<T>::ToFixedMatrix()
{
    int largest_vec_size = 0;
    for (const auto& row : rows) {
        if (row.second.size() > largest_vec_size) {
            largest_vec_size = row.second.size();
        }
    }
    std::ranges::for_each(rows, [largest_vec_size](auto& vec)
    {
        vec.second.resize(largest_vec_size);
    });
    std::vector<std::vector<T>> temp_vec;
    for (auto& row : rows) {
        temp_vec.push_back(row.second);
    }
    return Matrix<T>{n_rows, largest_vec_size, temp_vec};
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const DynamicMatrix<T>& matrix)
{
    for (int i = 0; i < matrix.n_rows; ++i) {
        const auto& row = matrix.Row(i);
        for (const auto& item : row) {
            stream << item << ' ';
        }
        stream << '\n';
    }
    return stream;
}

#endif //HALAL_DYNAMICMATRIX_HPP