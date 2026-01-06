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

template<typename IteratorType, ranges::view V, typename IterT = ranges::iterator_t<V>>
class eager_iterator {
    using vec_iter = IteratorType;
    vec_iter current;
public:
    using iterator_tag = std::random_access_iterator_tag;
    using difference_type = std::vector<IterT>::difference_type;
    using value_type = ranges::range_value_t<V>;
    using size_type = std::vector<IterT>::size_type;
    using reference = ranges::range_reference_t<V>;
    using const_reference = const ranges::range_reference_t<V>;
    using pointer = IterT;
    using allocator_type = std::vector<IterT>::allocator_type;

    eager_iterator() = default;
    explicit constexpr eager_iterator(vec_iter it) : current(it) {}

    constexpr reference operator*() { return **current; }
    constexpr pointer operator->() { return *current; }
    constexpr const_reference operator*() const { return **current; }
    constexpr pointer operator->() const { return *current; }

    constexpr eager_iterator& operator++() {
        ++current;
        return *this;
    }

    constexpr eager_iterator operator++(int) {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    constexpr eager_iterator& operator--() {
        --current;
        return *this;
    }

    constexpr eager_iterator operator--(int) {
        auto temp = *this;
        --(*this);
        return temp;
    }

    constexpr eager_iterator& operator+=(const difference_type rhs) {
        current += rhs;
        return *this;
    }

    constexpr friend eager_iterator operator+(const eager_iterator& lhs, const difference_type rhs) {
        auto temp = lhs;
        return temp += rhs;
    }

    constexpr eager_iterator& operator-=(const difference_type rhs) {
        current -= rhs;
        return *this;
    }

    constexpr friend eager_iterator operator-(const eager_iterator& lhs, const difference_type rhs) {
        auto temp = lhs;
        return temp -= rhs;
    }

    constexpr friend difference_type operator-(const eager_iterator& lhs, const eager_iterator& rhs) {
        return lhs.current - rhs.current;
    }

    constexpr reference operator[](const size_type idx) {
        return **(current + idx);
    }

   constexpr friend bool operator<(const eager_iterator& lhs, const eager_iterator& rhs) { return lhs.current < rhs.current; }
   constexpr friend bool operator>(const eager_iterator& lhs, const eager_iterator& rhs) { return lhs.current > rhs.current; }
   constexpr friend bool operator==(const eager_iterator& lhs, const eager_iterator& rhs) { return lhs.current == rhs.current; }
   constexpr friend bool operator!=(const eager_iterator& lhs, const eager_iterator& rhs) { return lhs.current != rhs.current; }
};

template<ranges::forward_range V, typename T = ranges::range_value_t<V>>
requires ranges::view<V>
class eager_view_base : public ranges::view_interface<eager_view_base<V>>
{
protected:
    using iter_t = ranges::iterator_t<V>;
    std::shared_ptr<std::vector<iter_t>> eager_view_;
public:
    using iterator = eager_iterator<typename std::vector<iter_t>::iterator, V>;
    using const_iterator = eager_iterator<typename std::vector<iter_t>::const_iterator, V>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    virtual ~eager_view_base() = default;

    constexpr iterator begin() { return iterator(eager_view_->begin()); }
    constexpr iterator end() { return iterator(eager_view_->end()); }
    constexpr const_iterator cbegin() { return const_iterator(eager_view_->cbegin()); }
    constexpr const_iterator cend() { return const_iterator(eager_view_->cend()); }
    constexpr reverse_iterator rbegin() { return reverse_iterator{end()}; }
    constexpr reverse_iterator rend() { return reverse_iterator{begin()}; }
    [[nodiscard]] constexpr size_t size() const { return eager_view_->size(); }
    constexpr iter_t at(size_t idx) { return eager_view_->at(idx); }
    constexpr iter_t operator[](size_t idx) { return eager_view_->at(idx); }
    constexpr auto erase(iterator first, iterator last) { eager_view_->erase(first.current, last.current); }
};

template<std::ranges::forward_range V, class Gen>
requires std::ranges::view<V> && std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
class sample_view final : public eager_view_base<V> {
    using view_base = eager_view_base<V>;

public:
    sample_view() = default;

    constexpr sample_view(V&& range, size_t sample_count, Gen&& gen) : base_(std::move(range))
    {
        std::vector<iter_t> iters;
        for (auto it = std::ranges::begin(base_); it != std::ranges::end(base_); ++it) {
            iters.push_back(it);
        }

        std::vector<iter_t> selected; selected.reserve(sample_count);
        std::sample(iters.begin(), iters.end(), std::back_inserter(selected), sample_count, std::move(gen));
        view_base::eager_view_ = std::make_shared<std::vector<iter_t>>(std::move(selected));
    }

private:
    V base_;
    using iter_t = eager_view_base<V>::iter_t;
    using distrib_t = std::uniform_real_distribution<>;
    using param_t = distrib_t::param_type;
};


template<ranges::input_range V>
requires ranges::view<V>
class concat_view final : public eager_view_base<V>
{
    using view_base = eager_view_base<V>;
    using iter_t = view_base::iter_t;
public:
    concat_view() = default;

    constexpr concat_view(V&& first, V&& second) : base_first_(std::move(first)), base_second_(std::move(second))
    {
        std::vector<iter_t> iters;
        for (auto it = ranges::begin(base_first_); it != ranges::end(base_first_); ++it) {
            iters.push_back(it);
        }
        for (auto it = ranges::begin(base_second_); it != ranges::end(base_second_); ++it) {
            iters.push_back(it);
        }
        view_base::eager_view_ = std::make_shared<std::vector<iter_t>>(std::move(iters));
    }

private:
    V base_first_;
    V base_second_;
};


template<ranges::forward_range V, std::indirect_unary_predicate<ranges::iterator_t<V>> F>
requires ranges::view<V>
class eager_filter_view final : public eager_view_base<V> {
    using view_base = eager_view_base<V>;
    using iter_t = view_base::iter_t;
public:
    using iterator = eager_iterator<typename std::vector<iter_t>::iterator, V>;
    using const_iterator = eager_iterator<typename std::vector<iter_t>::const_iterator, V>;

    eager_filter_view() = default;

    constexpr eager_filter_view(V&& range, const F& pred) : base_(std::move(range)) {
        std::vector<iter_t> iters;
        iters.reserve(ranges::size(base_));
        for (auto it = ranges::begin(base_); it != ranges::end(base_); ++it) {
            if (pred(*it)) {
                iters.push_back(it);
            }
        }
        view_base::eager_view_ = std::make_shared<std::vector<iter_t>>(std::move(iters));
    }

private:
    V base_;
};

template<ranges::random_access_range V> requires ranges::view<V>
class take_at final : public eager_view_base<V>
{
    using view_base = eager_view_base<V>;
    using iter_t = view_base::iter_t;
    V base_;
public:
    using iterator = eager_iterator<typename std::vector<iter_t>::iterator, V>;
    using const_iterator = eager_iterator<typename std::vector<iter_t>::const_iterator, V>;

    take_at() = default;

    template<ranges::forward_range R, typename T = ranges::range_value_t<R>> requires std::integral<T>
    constexpr take_at(V&& range, R&& indices) : base_(std::move(range))
    {
        std::vector<iter_t> iters;
        iters.reserve(ranges::size(base_));
        auto start = ranges::begin(base_);
        ranges::for_each(std::forward<R>(indices), [this, &iters, start](const auto& idx) {
            auto it = start + idx;
            iters.push_back(it);
        });
        view_base::eager_view_ = std::make_shared<std::vector<iter_t>>(std::move(iters));
    }
};

template<typename R, typename Gen>
sample_view(R&&, size_t, Gen&&) -> sample_view<std::views::all_t<R>, Gen>;

template<typename R>
concat_view(R&&, R&&) -> concat_view<ranges::views::all_t<R>>;

template<typename R, typename F>
eager_filter_view(R&&, F&&) -> eager_filter_view<ranges::views::all_t<R>, F>;

template<typename R>
take_at(R&&) -> take_at<ranges::views::all_t<R>>;

template<ranges::input_range In, typename Out> requires ranges::forward_range<In> || ranges::random_access_range<In>
void move_n(In&& inp, Out out, size_t n)
{
    auto start = ranges::begin(std::forward<In>(inp));
    if constexpr (ranges::random_access_range<In>) {
        for (auto it = start; it < start + n; ++it) {
            *out = std::move(*it);
            ++out;
        }
    } else {
        size_t i = 0;
        for (auto iter = start; i++ < n; ++iter) {
            *out = std::move(*iter);
            ++out;
        }
    }
}
#endif //HALAL_RANGESEXT_H