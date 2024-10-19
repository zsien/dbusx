#ifndef DBUSX_UTILS_H
#define DBUSX_UTILS_H

#include <tl/expected.hpp>

#include <type_traits>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <array>
#include <optional>
#include <utility>

#define check(x)                                                                                   \
    ({                                                                                             \
        int r = x;                                                                                 \
        errno = r < 0 ? -r : 0;                                                                    \
        printf(#x ": %m\n");                                                                       \
        if (r < 0) return false;                                                                   \
    })

template <typename T>
inline constexpr bool always_false_v = false;

template <typename T>
struct is_vector : public std::false_type {};

template <typename T>
struct is_vector<std::vector<T>> : public std::true_type {};
template <typename T>
struct is_vector<std::vector<T>&> : public std::true_type {};

template <typename T>
struct is_unordered_map : public std::false_type {};

template <typename K, typename V>
struct is_unordered_map<std::unordered_map<K, V>> : public std::true_type {};

template <typename T>
struct is_tuple : public std::false_type {};

template <typename... T>
struct is_tuple<std::tuple<T...>> : public std::true_type {};

template <typename T>
struct is_optional : public std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : public std::true_type {};

// get return type, the expected type is returned if use expected
template <typename T>
struct return_type {
    using type = T;
};

template <typename T, typename E>
struct return_type<tl::expected<T, E>> {
    using type = T;
};

// expansion pack
template <std::size_t N1, std::size_t... I1, std::size_t N2, std::size_t... I2>
static constexpr std::array<char, N1 + N2> concat(const std::array<char, N1> &a1,
                                                  const std::array<char, N2> &a2,
                                                  std::index_sequence<I1...> /* unused */,
                                                  std::index_sequence<I2...> /* unused */) {
    return {a1[I1]..., a2[I2]...};
}

template <std::size_t N1>
static constexpr auto concat(const std::array<char, N1> &a1) {
    return a1;
}

template <std::size_t N1, std::size_t N2, std::size_t... Ns>
static constexpr auto concat(const std::array<char, N1> &a1,
                             const std::array<char, N2> &a2,
                             const std::array<char, Ns> &...as) {
    return concat(concat(a1, a2, std::make_index_sequence<N1>{}, std::make_index_sequence<N2>{}),
                  as...);
}

// determine if they're POD type
template <typename... T>
struct all_pod;

template <typename T>
struct all_pod<T> {
    static constexpr bool value = std::is_trivial<T>::value && std::is_standard_layout<T>::value;
};

template <typename HEAD, typename... TAIL>
struct all_pod<HEAD, TAIL...> {
    static constexpr bool value = all_pod<HEAD>::value && all_pod<TAIL...>::value;
};

#endif // !DBUSX_UTILS_H
