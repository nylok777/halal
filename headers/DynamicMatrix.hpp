//
// Created by david on 10/23/25.
//

#ifndef HALAL_DYNAMICMATRIX_HPP
#define HALAL_DYNAMICMATRIX_HPP
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <ostream>

template <typename T>
class DynamicMatrix;

template <typename T>
auto operator<<(std::ostream& stream, const DynamicMatrix<T>& matrix) -> std::ostream&;

template <typename T>
class DynamicMatrix
{
public:
    DynamicMatrix() = default;

    explicit DynamicMatrix(const int n_rows);

    explicit DynamicMatrix(int n_rows, int n_cols);

    void PushBack(const int row, const T& value);

    template<std::ranges::forward_range R>
    requires std::same_as<std::ranges::range_value_t<R>, T>
    void PushBack(R&& row_range);

    void PopBack(const int row);

    void Insert(const int row, const int col, const T& value);

    void Resize(const int col, const int size);

    void Resize(const int size);

    auto Row(const int row) -> std::vector<T>&;

    auto Row(const int row) const -> const std::vector<T>&;

    auto Column(const int col) const -> std::vector<const T*>;

    auto Column(int col) -> std::vector<T*>;

    auto At(const int row, const int col) const -> const T&;

    auto At(const int row, const int col) -> T&;

    [[nodiscard]] auto RowCount() const -> int;

    [[nodiscard]] auto ColumnCount() const -> int;

    [[nodiscard]] auto Size() const -> int;

    [[nodiscard]] auto SizeOfRow(const int n_row) const -> int;

    friend auto operator<<<T>(std::ostream& stream, const DynamicMatrix<T>& matrix) -> std::ostream&;

private:
    std::map<int, std::vector<T>> rows;
    int n_rows;

public:
    class iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = value_type&;
        using pointer = value_type*;

        auto operator==(const iterator& other) const -> bool;

        auto operator!=(const iterator& other) const -> bool;

        auto operator++() -> iterator&;

        auto operator++(int) -> iterator;

        auto operator--() -> iterator&;

        auto operator--(int) -> iterator;

        auto operator*() -> T&;

        auto operator->() -> T*;

        friend auto DynamicMatrix::begin() -> iterator;

        friend auto DynamicMatrix::end() -> iterator;

    private:
        explicit iterator(std::map<int, std::vector<T>>* const current);

        iterator(std::vector<T>* const end_row, std::map<int, std::vector<T>>* const end);

        iterator(const iterator& it);

        std::map<int, std::vector<T>>::iterator current_row;
        std::vector<T>::iterator current_vec_iter;
        std::vector<T>::iterator current_vec_begin;
        std::vector<T>::iterator current_vec_end;
        std::map<int, std::vector<T>>::iterator matrix_end;
    };

    class const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = const value_type&;
        using pointer = const value_type*;

        auto operator==(const const_iterator& other) const -> bool;

        auto operator!=(const const_iterator& other) const -> bool;

        auto operator++() -> const_iterator&;

        auto operator++(int) -> const_iterator;

        auto operator*() -> const T&;

        auto operator->() -> const T*;

        friend auto DynamicMatrix::cbegin() const -> const_iterator;

        friend auto DynamicMatrix::cend() const -> const_iterator;

    private:
        explicit const_iterator(const std::map<int, std::vector<T>>* const current);

        const_iterator(const std::vector<T>* const end_row, const std::map<int, std::vector<T>>* const end);

        std::map<int, std::vector<T>>::const_iterator current_row;
        std::vector<T>::const_iterator current_vec_iter;
        std::vector<T>::const_iterator current_vec_end;
        std::map<int, std::vector<T>>::const_iterator matrix_end;
    };

    auto begin() -> iterator;

    auto end() -> iterator;

    auto begin() const -> const_iterator;

    auto end() const -> const_iterator;

    auto cbegin() const -> const_iterator;

    auto cend() const -> const_iterator;
};

template <typename T>
auto operator<<(std::ostream& stream, const DynamicMatrix<T>& matrix) -> std::ostream&
{
    for (int i = 0; i < matrix.n_rows; ++i) {
        for (const auto& row = matrix.Row(i); const auto& item : row) {
            stream << item << ' ';
        }
        stream << std::endl;
    }
    return stream;
}

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
DynamicMatrix<T>::DynamicMatrix(const int n_rows, const int n_cols) : n_rows(n_rows)
{
    std::map<int, std::vector<T>> matrix;
    for (int i = 0; i < n_rows; ++i) {
        std::vector<T> row(n_cols);
        matrix.insert(std::make_pair(i, row));
    }
    rows = matrix;
}

template <typename T>
void DynamicMatrix<T>::PushBack(const int row, const T& value) { rows.find(row)->second.push_back(value); }

template <typename T>
template <std::ranges::forward_range R>
requires std::same_as<std::ranges::range_value_t<R>, T>
void DynamicMatrix<T>::PushBack(R&& row_range)
{
    rows.emplace(++n_rows, std::vector<T>{std::ranges::begin(row_range), std::ranges::end(row_range)});
}

template <typename T>
void DynamicMatrix<T>::PopBack(const int row) { rows.extract(row).mapped().pop_back(); }


template <typename T>
void DynamicMatrix<T>::Insert(const int row, const int col, const T& value)
{
    rows.find(row)->second.at(col) = value;
}

template <typename T>
void DynamicMatrix<T>::Resize(const int col, const int size) { rows.find(col)->second.resize(size); }


template <typename T>
void DynamicMatrix<T>::Resize(const int size)
{
    for (auto& row : rows) {
        row.second.resize(size);
    }
}

template <typename T>
auto DynamicMatrix<T>::Row(const int row) -> std::vector<T>&
{
    return rows.find(row)->second;
}

template <typename T>
auto DynamicMatrix<T>::Row(const int row) const -> const std::vector<T>&
{
    return rows.find(row)->second;
}

template <typename T>
auto DynamicMatrix<T>::Column(const int col) const -> std::vector<const T*>
{
    std::vector<const T*> column;
    for (auto& row : rows) {
        if (col >= row.second.size()) continue;
        auto item_ptr = &row.second.at(col);
        column.push_back(item_ptr);
    }
    return column;
}

template <typename T>
auto DynamicMatrix<T>::Column(int col) -> std::vector<T*>
{
    std::vector<T*> column;
    for (auto& row : rows) {
        if (col >= row.second.size()) continue;
        auto item = &row.second.at(col);
        column.push_back(item);
    }
    return column;
}

template <typename T>
auto DynamicMatrix<T>::At(const int row, const int col) const -> const T&
{
    return rows.at(row).at(col);
}

template <typename T>
auto DynamicMatrix<T>::At(const int row, const int col) -> T&
{
    return rows.at(row).at(col);
}

template <typename T>
auto DynamicMatrix<T>::RowCount() const -> int
{
    return n_rows;
}

template <typename T>
auto DynamicMatrix<T>::ColumnCount() const -> int
{
    auto largest = std::ranges::max_element(
        rows,
        [](const auto& a, const auto& b) -> bool { return a.second.size() < b.second.size(); });
    return largest->second.size();
}

template <typename T>
auto DynamicMatrix<T>::Size() const -> int
{
    auto size = 0;
    for (const auto& row : rows) {
        size += row.second.size();
    }
    return size;
}

template <typename T>
auto DynamicMatrix<T>::SizeOfRow(const int n_row) const -> int { return rows.find(n_row)->second.size(); }

template <typename T>
auto DynamicMatrix<T>::iterator::operator==(const iterator& other) const -> bool
{
    return current_vec_iter == other.current_vec_iter;
}

template <typename T>
auto DynamicMatrix<T>::iterator::operator!=(const iterator& other) const -> bool { return !(*this == other); }

template <typename T>
auto DynamicMatrix<T>::iterator::operator++() -> iterator&
{
    ++current_vec_iter;
    if (current_vec_iter == current_vec_end) {
        ++current_row;
        if (current_row == matrix_end) return *this;
        current_vec_iter = current_row->second.begin();
        current_vec_end = current_row->second.end();
    }
    return *this;
}

template <typename T>
auto DynamicMatrix<T>::iterator::operator++(int) -> iterator
{
    iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
auto DynamicMatrix<T>::iterator::operator--() -> iterator&
{
    --current_vec_iter;
    if (current_vec_iter == current_vec_begin) {
        --current_row;
        current_vec_iter = current_row->second.begin();
        current_vec_end = current_row->second.end();
    }
    return *this;
}

template <typename T>
auto DynamicMatrix<T>::iterator::operator--(int) -> iterator
{
    auto temp = *this;
    --(*this);
    return temp;
}

template <typename T>
auto DynamicMatrix<T>::iterator::operator*() -> T&
{
    return *current_vec_iter;
}

template <typename T>
auto DynamicMatrix<T>::iterator::operator->() -> T*
{
    return current_vec_iter;
}

template <typename T>
DynamicMatrix<T>::iterator::iterator(std::map<int, std::vector<T>>* const current)
    :
    current_row(current->begin()),
    current_vec_iter(current->begin()->second.begin()),
    current_vec_begin(current->begin()->second.begin()),
    current_vec_end(current->begin()->second.end()),
    matrix_end(current->end()) {}

template <typename T>
DynamicMatrix<T>::iterator::iterator(std::vector<T>* const end_row, std::map<int, std::vector<T>>* const end)
    :
    current_row(end->end()),
    current_vec_iter(end_row->end()),
    current_vec_end(end_row->end()),
    matrix_end(end->end()) {}

template <typename T>
DynamicMatrix<T>::iterator::iterator(const iterator& it)
    :
    current_row(it.current_row),
    current_vec_iter(it.current_vec_iter),
    current_vec_begin(it.current_vec_begin),
    current_vec_end(it.current_vec_end),
    matrix_end(it.matrix_end) {}

template <typename T>
auto DynamicMatrix<T>::const_iterator::operator==(const const_iterator& other) const -> bool
{
    return current_vec_iter == other.current_vec_iter;
}

template <typename T>
auto DynamicMatrix<T>::const_iterator::operator!=(const const_iterator& other) const -> bool
{
    return !(*this == other);
}

template <typename T>
auto DynamicMatrix<T>::const_iterator::operator++() -> const_iterator&
{
    ++current_vec_iter;
    if (current_vec_iter == current_vec_end) {
        ++current_row;
        if (current_row == matrix_end) return *this;
        current_vec_iter = current_row->second.cbegin();
        current_vec_end = current_row->second.cend();
    }
    return *this;
}

template <typename T>
auto DynamicMatrix<T>::const_iterator::operator++(int) -> const_iterator
{
    const_iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
auto DynamicMatrix<T>::const_iterator::operator*() -> const T&
{
    return *current_vec_iter;
}

template <typename T>
auto DynamicMatrix<T>::const_iterator::operator->() -> const T*
{
    return current_vec_iter;
}

template <typename T>
DynamicMatrix<T>::const_iterator::const_iterator(const std::map<int, std::vector<T>>* const current)
    :
    current_row(current->cbegin()),
    current_vec_iter(current->cbegin()->second.cbegin()),
    current_vec_end(current->cbegin()->second.cend()),
    matrix_end(current->cend()) {}

template <typename T>
DynamicMatrix<T>::const_iterator::const_iterator(
    const std::vector<T>* const end_row,
    const std::map<int, std::vector<T>>* const end)
    :
    current_row(end->cend()),
    current_vec_iter(end_row->cend()),
    current_vec_end(end_row->cend()),
    matrix_end(end->cend()) {}

template <typename T>
auto DynamicMatrix<T>::begin() -> iterator { return iterator{&rows}; }

template <typename T>
auto DynamicMatrix<T>::end() -> iterator { return iterator{&rows.rbegin()->second, &rows}; }

template <typename T>
auto DynamicMatrix<T>::begin() const -> const_iterator { return cbegin(); }

template <typename T>
auto DynamicMatrix<T>::end() const -> const_iterator { return cend(); }

template <typename T>
auto DynamicMatrix<T>::cbegin() const -> const_iterator { return const_iterator{&rows}; }

template <typename T>
auto DynamicMatrix<T>::cend() const -> const_iterator
{
    return const_iterator{&rows.rbegin()->second, &rows};
}

#endif //HALAL_DYNAMICMATRIX_HPP
