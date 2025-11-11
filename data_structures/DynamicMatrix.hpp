//
// Created by david on 10/23/25.
//

#ifndef HALAL_DYNAMICMATRIX_HPP
#define HALAL_DYNAMICMATRIX_HPP
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>

template<typename T>
class DynamicMatrix;

template<typename T>
std::ostream& operator<<(std::ostream& stream, const DynamicMatrix<T>& matrix);


template<typename T>
class DynamicMatrix
{
public:
    explicit DynamicMatrix(const int n_rows);

public:
    void PushBack(const int n_row, const T& value);

    void PopBack(const int n_row);

    void Insert(const int n_row, const int n_col, const T& value);

    void Resize(const int n_row, const int size);

    void Resize(const int size);

public:
    std::vector<T>& Row(const int n_row);

    const std::vector<T>& Row(const int n_row) const;

    std::vector<const T*> Column(const int n_col) const;

    std::vector<std::vector<const T*>> Columns() const;

    const T& At(const int row_i, const int col_i) const;

public:
    [[nodiscard]] int RowCount() const;

    [[nodiscard]] int ColumnCount() const;

    [[nodiscard]] int Size() const;

    [[nodiscard]] int SizeOfRow(const int n_row) const;

public:
    friend std::ostream& operator<<<T>(std::ostream& stream, const DynamicMatrix<T>& matrix);

private:
    std::map<int, std::vector<T>> rows;
    int n_rows;

public:
    class row_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = value_type&;
        using pointer = value_type*;

    public:
        bool operator==(const row_iterator& other) const;

        bool operator!=(const row_iterator& other) const;

        row_iterator& operator++();

        row_iterator operator++(int);

        T& operator*();

        friend row_iterator DynamicMatrix::begin();

        friend row_iterator DynamicMatrix::end();

    private:
        explicit row_iterator(std::map<int, std::vector<T>>* const current);

        row_iterator(std::vector<T>* const end_row, std::map<int, std::vector<T>>* const end);

    private:
        std::map<int, std::vector<T>>::iterator current_row;
        std::vector<T>::iterator current_vec_iter;
        std::vector<T>::iterator current_vec_end;
        std::map<int, std::vector<T>>::iterator matrix_end;
    };


    class row_const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = const value_type&;
        using pointer = const value_type*;

    public:
        bool operator==(const row_const_iterator& other) const;

        bool operator!=(const row_const_iterator& other) const;

        row_const_iterator operator++();

        const T& operator*();


        friend row_const_iterator DynamicMatrix::begin() const;

        friend row_const_iterator DynamicMatrix::end() const;

    private:
        explicit row_const_iterator(const std::map<int, std::vector<T>>* const current);

        row_const_iterator(const std::vector<T>* const end_row, const std::map<int, std::vector<T>>* const end);

    private:
        std::map<int, std::vector<T>>::const_iterator current_row;
        std::vector<T>::const_iterator current_vec_iter;
        std::vector<T>::const_iterator current_vec_end;
        std::map<int, std::vector<T>>::const_iterator matrix_end;
    };


    row_iterator begin();

    row_iterator end();

    row_const_iterator begin() const;

    row_const_iterator end() const;
};


template<typename T>
DynamicMatrix<T>::row_const_iterator::row_const_iterator(const std::map<int, std::vector<T>>* const current)
    :
    current_row(current->cbegin()),
    current_vec_iter(current->cbegin()->second.cbegin()),
    current_vec_end(current->cbegin()->second.cend()),
    matrix_end(current->cend()) {}


template<typename T>
DynamicMatrix<T>::row_const_iterator::row_const_iterator(
    const std::vector<T>* const end_row,
    const std::map<int, std::vector<T>>* const end)
    :
    current_row(end->cend()),
    current_vec_iter(end_row->cend()),
    current_vec_end(end_row->cend()),
    matrix_end(end->cend()) {}

template<typename T>
bool DynamicMatrix<T>::row_const_iterator::operator==(const row_const_iterator& other) const
{
    return current_vec_iter == other.current_vec_iter;
}

template<typename T>
bool DynamicMatrix<T>::row_const_iterator::operator!=(const row_const_iterator& other) const
{
    return !(*this == other);
}


template<typename T>
const T& DynamicMatrix<T>::row_const_iterator::operator*() { return *current_vec_iter; }

template<typename T>
DynamicMatrix<T>::row_const_iterator DynamicMatrix<T>::row_const_iterator::operator++()
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


template<typename T>
DynamicMatrix<T>::row_const_iterator DynamicMatrix<T>::begin() const { return row_const_iterator{&rows}; }

template<typename T>
DynamicMatrix<T>::row_const_iterator DynamicMatrix<T>::end() const
{
    return row_const_iterator{&rows.rbegin()->second, &rows};
}

template<typename T>
DynamicMatrix<T>::row_iterator::row_iterator(std::map<int, std::vector<T>>* const current)
    : current_row(current->begin()),
      current_vec_iter(current->begin()->second.begin()),
      current_vec_end(current->begin()->second.end()),
      matrix_end(current->end()) {}

template<typename T>
DynamicMatrix<T>::row_iterator::row_iterator(std::vector<T>* const end_row, std::map<int, std::vector<T>>* const end)
    :
    current_row(end->end()),
    current_vec_iter(end_row->end()),
    current_vec_end(end_row->end()),
    matrix_end(end->end()) {}


template<typename T>
T& DynamicMatrix<T>::row_iterator::operator*() { return *current_vec_iter; }

template<typename T>
DynamicMatrix<T>::row_iterator DynamicMatrix<T>::row_iterator::operator++(int)
{
    row_iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
DynamicMatrix<T>::row_iterator& DynamicMatrix<T>::row_iterator::operator++()
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

template<typename T>
bool DynamicMatrix<T>::row_iterator::operator==(const row_iterator& other) const
{
    return current_vec_iter == other.current_vec_iter;
}

template<typename T>
bool DynamicMatrix<T>::row_iterator::operator!=(const row_iterator& other) const { return !(*this == other); }

template<typename T>
DynamicMatrix<T>::row_iterator DynamicMatrix<T>::begin() { return row_iterator{&rows}; }

template<typename T>
DynamicMatrix<T>::row_iterator DynamicMatrix<T>::end() { return row_iterator{&rows.rbegin()->second, &rows}; }

template<typename T>
DynamicMatrix<T>::DynamicMatrix(const int n_rows) : n_rows(n_rows)
{
    std::map<int, std::vector<T>> matrix_map;
    for (int i = 0; i < n_rows; ++i) {
        std::vector<T> row;
        matrix_map.insert(std::make_pair(i, row));
    }
    rows = matrix_map;
}

template<typename T>
void DynamicMatrix<T>::PushBack(const int n_row, const T& value) { rows.find(n_row)->second.push_back(value); }

template<typename T>
void DynamicMatrix<T>::Insert(const int n_row, const int n_col, const T& value)
{
    rows.find(n_row)->second.at(n_col) = value;
}


template<typename T>
void DynamicMatrix<T>::PopBack(const int n_row) { rows.extract(n_row).mapped().pop_back(); }

template<typename T>
void DynamicMatrix<T>::Resize(const int size) { for (auto& row : rows) { row.second.resize(size); } }

template<typename T>
void DynamicMatrix<T>::Resize(const int n_row, const int size) { rows.find(n_row)->second.resize(size); }


template<typename T>
std::vector<T>& DynamicMatrix<T>::Row(const int n_row) { return rows.find(n_row)->second; }

template<typename T>
const std::vector<T>& DynamicMatrix<T>::Row(const int n_row) const { return rows.find(n_row)->second; }

template<typename T>
std::vector<const T*> DynamicMatrix<T>::Column(const int n_col) const
{
    std::vector<const T*> col;
    for (auto& row : rows) {
        if (n_col >= row.second.size()) continue;
        auto item_ptr = &row.second.at(n_col);
        col.push_back(item_ptr);
    }
    return col;
}

template<typename T>
std::vector<std::vector<const T*>> DynamicMatrix<T>::Columns() const
{
    std::vector<std::vector<const T*>> cols;
    for (int i = 0; i < ColumnCount(); ++i) { cols.push_back(Column(i)); }
    return cols;
}


template<typename T>
int DynamicMatrix<T>::RowCount() const { return n_rows; }

template<typename T>
int DynamicMatrix<T>::ColumnCount() const
{
    auto largest = std::ranges::max_element(
        rows,
        [](const auto& a, const auto& b) { return a.second.size() < b.second.size(); });
    return largest->second.size();
}


template<typename T>
int DynamicMatrix<T>::Size() const
{
    auto size = 0;
    for (const auto& row : rows) { size += row.second.size(); }
    return size;
}

template<typename T>
int DynamicMatrix<T>::SizeOfRow(const int n_row) const { return rows.find(n_row)->second.size(); }

template<typename T>
const T& DynamicMatrix<T>::At(const int row_i, const int col_i) const { return rows.at(row_i).at(col_i); }

template<typename T>
std::ostream& operator<<(std::ostream& stream, const DynamicMatrix<T>& matrix)
{
    for (int i = 0; i < matrix.n_rows; ++i) {
        const auto& row = matrix.Row(i);
        for (const auto& item : row) { stream << item << ' '; }
        //stream << '\n';
    }
    return stream;
}

#endif //HALAL_DYNAMICMATRIX_HPP
