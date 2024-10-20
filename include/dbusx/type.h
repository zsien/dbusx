#ifndef DBUSX_TYPE_H
#define DBUSX_TYPE_H

#include "utils.h"
#include "typedef.h"

#include <any>
#include <array>
#include <functional>
#include <string>
#include <type_traits>

#include <stdint.h>

namespace dbusx {

namespace {
template <typename T, bool general_concept = false>
struct tuple_items_type {};
} // namespace

/*!
  @brief Get D-Bus signature string from a C++ type
 */
template <typename T, bool general_concept = false>
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
            return concat(std::array{'a'}, type<typename Type::value_type, general_concept>::signature);
        } else if constexpr (is_unordered_map<Type>::value) {
            // dict
            if constexpr (general_concept) {
                return concat(std::array{'a', 'e'},
                              type<typename Type::key_type, general_concept>::signature,
                              type<typename Type::mapped_type, general_concept>::signature);
            } else {
                return concat(std::array{'a', '{'},
                              type<typename Type::key_type, general_concept>::signature,
                              type<typename Type::mapped_type, general_concept>::signature,
                              std::array{'}'});
            }
        } else if constexpr (is_tuple<Type>::value) {
            // struct
            // top-level cv-qualifiers and reference removed
            if constexpr (general_concept) {
                return concat(std::array{'r'}, tuple_items_type<Type, general_concept>::signature);
            } else {
                return concat(std::array{'('}, tuple_items_type<Type, general_concept>::signature, std::array{')'});
            }
        } else if constexpr (std::is_same<Type, std::any>::value) {
            return std::array{'v'};
        } else if constexpr (std::is_void<T>::value) {
            return std::array<char, 0>{};
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    });

    static constexpr auto signature_str = std::string(signature.cbegin(), signature.cend());
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

    static constexpr auto signature_str = std::string(signature.cbegin(), signature.cend());
};

namespace {
template <typename... T, bool general_concept>
struct tuple_items_type<std::tuple<T...>, general_concept> {
    static constexpr auto signature = concat(type<T, general_concept>::signature...);
};
} // namespace

} // namespace dbusx

#endif // !DBUSX_TYPE_H
