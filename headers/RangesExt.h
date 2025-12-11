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
        sampled = std::make_shared<std::vector<range_iter_t>>(selected);
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

#endif //HALAL_RANGESEXT_H