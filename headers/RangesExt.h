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
        using iter_category = std::forward_iterator_tag;
        using diff_t = std::vector<range_iter_t>::difference_type;
        using value_t = std::ranges::range_value_t<R>;
        using ref_t = std::ranges::range_reference_t<R>;
        using vec_iter = std::vector<range_iter_t>::iterator;

    private:
        vec_iter current;

    public:
        iterator() = default;
        explicit iterator(vec_iter it) : current(it) {}

        ref_t operator*() const
        {
            return **current;
        }

        iterator& operator++()
        {
            ++current;
            return *this;
        }

        iterator operator++(int)
        {
            auto past = *this;
            ++(*this);
            return past;
        }

        bool operator==(const iterator& other) const { return current == other.current;}
    };

    auto begin() { return iterator{sampled->begin()}; }

    auto end() { return iterator{sampled->end()}; }
};

template<typename R, typename Gen>
sample_view(R&&, size_t, Gen&&) -> sample_view<std::views::all_t<R>, std::mt19937>;

template<ranges::input_range V1, ranges::input_range V2>
requires ranges::view<V1> &&
    ranges::view<V2> &&
    std::same_as<V1, V2>
class concat_view : ranges::view_interface<concat_view<V1, V2>>
{
public:
    concat_view() = default;

    concat_view(V1&& first, V2&& second) : base_first(first), base_second(second)
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
    V1 base_first;
    V2 base_second;
    using iter_t = ranges::iterator_t<V1>;
    std::shared_ptr<std::vector<iter_t>> concat;

public:
    class iterator
    {
    public:
        using iter_category = std::forward_iterator_tag;
        using diff_t = std::vector<iter_t>::difference_type;
        using ref_t = ranges::range_reference_t<V1>;
        using value_t = ranges::range_value_t<V1>;

        using vec_iter = std::vector<iter_t>::iterator;

        iterator() = default;

        explicit iterator(vec_iter it) : current(it) {}

        ref_t operator*()
        {
            return **current;
        }

        iterator& operator++()
        {
            ++current;
            return *this;
        }

        iterator operator++(int)
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const
        {
            return current == other.current;
        }

    private:
        vec_iter current;
    };

    auto begin() { return iterator(concat->begin()); }
    auto end() { return iterator(concat->end()); }
};

template<typename R1, typename R2>
concat_view(R1&&, R2&&) -> concat_view<ranges::views::all_t<R1>, ranges::views::all_t<R2>>;

#endif //HALAL_RANGESEXT_H