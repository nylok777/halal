//
// Created by david on 12/10/25.
//

#ifndef HALAL_RANGESEXT_H
#define HALAL_RANGESEXT_H
#include <iterator>
#include <random>
#include <ranges>

struct sample_view_fn
{
    template<std::ranges::input_range R, class Gen>
    requires std::ranges::forward_range<R> &&
        std::uniform_random_bit_generator<std::remove_reference_t<Gen>>
    constexpr auto operator()(R&& range, std::ranges::range_difference_t<R> n, Gen&& gen) const
    {
        using diff_t = std::ranges::range_difference_t<R>;
        using iter_t = std::ranges::iterator_t<R>;
        using distrib_t = std::uniform_int_distribution<diff_t>;
        using param_t = distrib_t::param_type;
        distrib_t dist{};
        auto first = std::ranges::begin(range);
        auto last = std::ranges::end(range);
        std::vector<iter_t> temp;
        if constexpr (std::ranges::forward_range<R>) {
            diff_t i = 0;
            auto rest = std::ranges::distance(range);
            for (n = std::ranges::min(n, rest); n != 0; ++first) {
                if (dist(gen, param_t(0, --rest) < n)) {
                    temp[i++] = range;
                    --n;
                }
            }
            return std::ranges::owning_view(std::move(temp));
        }
        else {
            diff_t temp_idx{};
            //std::uniform_real_distribution<> const real_dist;
            while (temp_idx < n) {
                for (auto it = first; it != last; ++it) {
                    if (dist(gen, param_t(0, n)) < n/2) temp[temp_idx++] = it;
                }
            }
            return std::ranges::owning_view(std::move(temp));
        }
    }
};
namespace ranges_ext::views
{
inline constexpr sample_view_fn sample{};

}

#endif //HALAL_RANGESEXT_H