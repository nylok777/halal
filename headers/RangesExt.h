//
// Created by david on 12/10/25.
//

#ifndef HALAL_RANGESEXT_H
#define HALAL_RANGESEXT_H
#include <algorithm>
#include <iterator>
#include <memory>
#include <random>
#include <ranges>
#include <iterator>

namespace ranges = std::ranges;

template<std::ranges::input_range R, class Gen>
requires std::ranges::view<R> && std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
class sample_view : public std::ranges::view_interface<sample_view<R, Gen>>
{
public:
    sample_view() = default;

    sample_view(R&& range, size_t sample_count, Gen&& gen) : base(range)
    {
        std::vector<range_iter_t> iters;
        for (auto it = std::ranges::begin(base); it != std::ranges::end(base); ++it) {
            iters.push_back(it);
        }

        std::vector<range_iter_t> selected; selected.reserve(sample_count);
        std::sample(iters.begin(), iters.end(), std::back_inserter(selected), sample_count, gen);
        sampled = std::make_shared<std::vector<range_iter_t>>(std::move(selected));
    }

private:
    using range_iter_t = std::ranges::iterator_t<R>;
    using distrib_t = std::uniform_real_distribution<>;
    using param_t = distrib_t::param_type;
    R base;
    std::shared_ptr<std::vector<range_iter_t>> sampled;

public:
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::vector<range_iter_t>::difference_type;
        using value_type = std::ranges::range_value_t<R>;
        using reference = std::ranges::range_reference_t<R>;
        using pointer = value_type*;

        using vec_iter = std::vector<range_iter_t>::iterator;

    private:
        vec_iter current;

    public:
        iterator() = default;
        explicit iterator(vec_iter it) : current(it) {}

        auto operator*() const -> reference { return **current; }

        auto operator->() const -> pointer { return current; }

        auto operator*() -> reference { return **current; }

        auto operator->() -> pointer { return current; }

        auto operator++() -> iterator&
        {
            ++current;
            return *this;
        }

        auto operator++(int) -> iterator
        {
            auto past = *this;
            ++(*this);
            return past;
        }

        auto operator==(const iterator& other) const -> bool { return current == other.current;}
        auto operator<(const iterator& other) const -> bool { return current < other.current; }
    };

    auto begin() { return iterator{sampled->begin()}; }

    auto end() { return iterator{sampled->end()}; }

    auto at(size_t idx) { return sampled->at(idx); }

    auto size() const { return sampled->size(); }
};

template<typename R, typename Gen>
sample_view(R&&, size_t, Gen&&) -> sample_view<std::views::all_t<R>, Gen>;

template<ranges::input_range V>
requires ranges::view<V>
class concat_view : public ranges::view_interface<concat_view<V>>
{
public:
    using range_value_t = ranges::range_value_t<V>;

    concat_view() = default;

    concat_view(V&& first, V&& second) : base_first(first), base_second(second)
    {
        std::vector<iter_t> iters;
        for (auto it = ranges::begin(base_first); it != ranges::end(base_first); ++it) {
            iters.push_back(it);
        }
        for (auto it = ranges::begin(base_second); it != ranges::end(base_second); ++it) {
            iters.push_back(it);
        }
        concat = std::make_shared<std::vector<iter_t>>(std::move(iters));
    }

private:
    V base_first;
    V base_second;
    using iter_t = ranges::iterator_t<V>;
    std::shared_ptr<std::vector<iter_t>> concat;

public:
    class iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::vector<iter_t>::difference_type;
        using reference = ranges::range_reference_t<V>;
        using value_type = ranges::range_value_t<V>;
        using size_type = std::vector<iter_t>::size_type;
        using pointer = std::vector<iter_t>::pointer;
        using allocator_type = std::vector<iter_t>::allocator_type;

        using vec_iter = std::vector<iter_t>::iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;

        iterator() = default;

        explicit iterator(vec_iter it) : current(it) {}

        iterator(const iterator&) = default;

        auto operator*() -> reference { return **current; }

        auto operator->() -> pointer { return current; }

        auto operator*() const -> reference { return **current; }

        auto operator->() const -> pointer { return current; }

        auto operator[](size_type idx) -> value_type&
        {
            return **(current + idx);
        }

        auto operator[](size_type idx) const -> value_type&
        {
            return **(current + idx);
        }

        friend auto operator-(const iterator& first, const iterator& last) -> difference_type
        {
            return first.current - last.current;
        }

        auto operator++() -> iterator&
        {
            ++current;
            return *this;
        }

        auto operator++(int) -> iterator
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        auto operator--() -> iterator&
        {
            --current;
            return *this;
        }

        auto operator--(int) -> iterator
        {
            auto temp = *this;
            --(*this);
            return temp;
        }

        auto operator+=(const difference_type rhs) -> iterator&
        {
            current += rhs;
            return *this;
        }

        friend auto operator+(const iterator& lhs, const difference_type rhs) -> iterator
        {
            iterator temp = lhs;
            return temp += rhs;
        }

        auto operator-=(const difference_type rhs) -> iterator&
        {
            current -= rhs;
            return *this;
        }

        friend auto operator-(const iterator& lhs, const difference_type rhs) -> iterator
        {
            iterator temp = lhs;
            return temp -= rhs;
        }

        auto operator==(const iterator& other) const -> bool
        {
            return current == other.current;
        }

        auto operator!=(const iterator& other) const -> bool
        {
            return current != other.current;
        }

        auto operator>(const iterator& other) const -> bool
        {
            return current > other.current;
        }

        auto operator<(const iterator& other) const -> bool
        {
            return current < other.current;
        }

    private:
        vec_iter current;
    };

    //using iterator_t = iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;

    auto begin()  { return iterator(concat->begin()); }
    auto end() { return iterator(concat->end()); }
    auto rbegin() { return reverse_iterator{end()}; }
    auto rend() { return reverse_iterator{begin()}; }
    auto size() const { return concat->size(); }
    auto at(size_t idx) { return concat->at(idx); }
    auto erase(iterator first, iterator last) { concat->erase(first.current, last.current); }
};

template<typename R>
concat_view(R&&, R&&) -> concat_view<ranges::views::all_t<R>>;

#endif //HALAL_RANGESEXT_H