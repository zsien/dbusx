#ifndef DBUSX_TYPE_H
#define DBUSX_TYPE_H

#include <any>
#include <array>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils.h"
#include "typedef.h"

namespace dbusx {

namespace {
template <typename T>
struct tuple_type {};
} // namespace

/*!
  @brief Get D-Bus signature string from a C++ type
 */
template <typename T>
struct type {
    static constexpr auto signature = std::invoke([] {
        using Type = std::remove_cvref_t<T>;

        if constexpr (std::is_same<Type, char>::value) {
            return std::array{'y'};
        } else if constexpr (std::is_same<Type, bool>::value) {
            return std::array{'b'};
        } else if constexpr (std::is_same<Type, int16_t>::value) {
            return std::array{'n'};
        } else if constexpr (std::is_same<Type, uint16_t>::value) {
            return std::array{'q'};
        } else if constexpr (std::is_same<Type, int32_t>::value) {
            return std::array{'i'};
        } else if constexpr (std::is_same<Type, uint32_t>::value) {
            return std::array{'u'};
        } else if constexpr (std::is_same<Type, int64_t>::value) {
            return std::array{'x'};
        } else if constexpr (std::is_same<Type, uint64_t>::value) {
            return std::array{'t'};
        } else if constexpr (std::is_same<Type, double>::value) {
            return std::array{'d'};
        } else if constexpr (std::is_same<Type, fd>::value) {
            return std::array{'h'};
        } else if constexpr (std::is_same<Type, std::string>::value) {
            return std::array{'s'};
        } else if constexpr (std::is_same<Type, object_path>::value) {
            return std::array{'o'};
        } else if constexpr (std::is_same<Type, dbusx::signature>::value) {
            return std::array{'g'};
        } else if constexpr (is_vector<Type>::value) {
            // array
            return concat(std::array{'a'}, type<typename Type::value_type>::signature);
        } else if constexpr (is_unordered_map<Type>::value) {
            // dict
            return concat(std::array{'{'},
                          type<typename Type::key_type>::signature,
                          type<typename Type::mapped_type>::signature,
                          std::array{'}'});
        } else if constexpr (is_tuple<Type>::value) {
            // struct
            // top-level cv-qualifiers and reference removed
            return tuple_type<Type>::signature;
        } else if constexpr (std::is_same<Type, std::any>::value) {
            return std::array{'v'};
        } else if constexpr (std::is_void<T>::value) {
            return std::array<char, 0>{};
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    });

    static constexpr auto signature_nt = concat(signature, std::array{'\0'});
};

/*!
  @brief Get D-Bus signature string from C++ types
 */
template <typename... T>
struct types {
    static constexpr auto signature = std::invoke([] {
        if constexpr (sizeof...(T) == 0) {
            return std::array<char, 0>();
        } else {
            return concat(type<T>::signature...);
        }
    });

    static constexpr auto signature_nt = concat(signature, std::array{'\0'});
};

namespace {
template <typename... T>
struct tuple_type<std::tuple<T...>> {
    static constexpr auto signature = concat(std::array{'('},
                                             type<T>::signature...,
                                             std::array{')'});
};
} // namespace

} // namespace dbusx

#endif // !DBUSX_TYPE_H
