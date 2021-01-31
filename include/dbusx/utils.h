#ifndef DBUSX_UTILS_H
#define DBUSX_UTILS_H

#include <array>
#include <utility>

template <typename T>
inline constexpr bool always_false_v = false;

// Expansion pack
template <std::size_t N1, std::size_t... I1, std::size_t N2, std::size_t... I2>
static constexpr std::array<char, N1 + N2>
concat(const std::array<char, N1> &a1,
       const std::array<char, N2> &a2,
       std::index_sequence<I1...> /*unused*/,
       std::index_sequence<I2...> /*unused*/) {
    return {a1[I1]..., a2[I2]...};
}

template <std::size_t N1>
static constexpr auto concat(const std::array<char, N1> &a1) {
    return a1;
}

template <std::size_t N1, std::size_t N2, std::size_t... Ns>
static constexpr auto concat(const std::array<char, N1> &a1,
                             const std::array<char, N2> &a2,
                             const std::array<char, Ns> &... as) {
    return concat(concat(a1,
                         a2,
                         std::make_index_sequence<N1>{},
                         std::make_index_sequence<N2>{}),
                  as...);
}

#endif // !DBUSX_UTILS_H
