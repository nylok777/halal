//
// Created by david on 12/24/25.
//

#ifndef HALAL_CONCEPTSEXT_H
#define HALAL_CONCEPTSEXT_H
#include <concepts>

template <typename T>
concept Numeric = std::integral<std::remove_cvref_t<T>> ||
    std::floating_point<std::remove_cvref_t<T>>;

#endif //HALAL_CONCEPTSEXT_H