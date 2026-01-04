//
// Created by david on 1/3/26.
//

#ifndef HALAL_MATRIX_HPP
#define HALAL_MATRIX_HPP
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <concepts>

template<typename T>
class Array
{
    using Iterator = std::vector<T>::iterator;
    using ConstIterator = std::vector<T>::const_iterator;
public:
    explicit Array(size_t size) : vec(size), size(size) {}

    template<typename Range> requires std::ranges::forward_range<Range>
    explicit Array(Range&& range) : vec(std::forward<Range>(range)), size(vec.size()) {}

    template<std::input_iterator First, std::sentinel_for<First> Last>
    Array(First first, Last last) : vec(first, last), size(vec.size()) {}

    template<typename Tp>
    void push_back(Tp&& item)
    {
        if (idx < size) {
            vec.at(idx++) = std::forward<Tp>(item);
        } else {
            throw std::out_of_range{"Index out of range"};
        }
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (idx < size) {
            vec.at(idx++) = T{std::forward<Args>(args)...};
        } else {
            throw std::out_of_range{"Index out of range"};
        }
    }

    void pop_back()
    {
        vec.pop_back();
        --idx;
    }

    explicit operator std::vector<T>() { return vec; }

    T& back() { return vec.back(); }

    T& back() const { return vec.back(); }

    T& operator[](size_t i) { return vec.at(i); }

    T& operator[](size_t i) const { return vec.at(i); }

    Iterator begin() { return vec.begin(); }
    Iterator end() { return vec.end(); }
    ConstIterator begin() const { return vec.begin(); }
    ConstIterator end() const { return vec.end(); }
    ConstIterator cbegin() { return vec.cbegin(); }
    ConstIterator cend() { return vec.cend(); }

private:
    std::vector<T> vec;
    size_t size;
    size_t idx = 0;
};

template<typename T>
class Matrix
{
    size_t n_rows;
    size_t n_columns;
    std::unordered_map<size_t, Array<T>> rows;
public:
    Matrix(const size_t n_rows, size_t n_columns) : n_rows(n_rows), n_columns(n_columns)
    {
        for (size_t i = 0; i < n_rows; ++i) {
            rows.emplace(i, Array<T>{n_columns});
        }
    }

    template<typename Range> requires std::ranges::forward_range<Range>
    explicit Matrix(size_t n_rows, Range&& range) : n_rows(n_rows), n_columns(std::ranges::size(range))
    {
        auto row = std::forward<Range>(range);
        for (size_t i = 0; i < n_rows; ++i) {
            rows.emplace(i, Array<T>{row});
        }
    }

    T& operator()(size_t i, size_t j) { return rows.at(i)[j]; }
    T& operator()(size_t i, size_t j) const { return rows.at(i)[j]; }

    template<typename Tp>
    void insert(size_t i, size_t j, Tp&& item) { rows.at(i)[j] = std::forward<Tp>(item); }

    template<typename Range> requires std::ranges::forward_range<Range>
    void insert_range(size_t row_i, Range&& range)
    {
        auto& row = rows.at(row_i);
        if (std::ranges::size(range) > n_columns) {
            auto last = std::ranges::begin(range);
            if constexpr (std::ranges::random_access_range<Range>) {
                last += n_columns;
            } else {
                for (size_t i = 0; i < n_columns; ++i) {
                    ++last;
                }
            }
            row = Array<T>{std::ranges::begin(std::forward<Range>(range)), last};
        }
        row = Array<T>{std::forward<Range>(range)};
    }

    Array<T>& row(size_t i) { return rows.at(i); }

    template<typename Tp>
    void push_back(size_t row_i, Tp&& item) { rows.at(row_i).push_back(std::forward<Tp>(item)); }

    template<typename... Args>
    void emplace_back(size_t row_i, Args&&... args) { rows.at(row_i).emplace_back(std::forward<Args>(args)...); }
};

#endif //HALAL_MATRIX_HPP