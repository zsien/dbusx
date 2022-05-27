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
#include "object_path.h"
#include "signature.h"

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
        using T1 = std::remove_cvref_t<T>;

        if constexpr (std::is_same<T1, char>::value) {
            return std::array{'y'};
        } else if constexpr (std::is_same<T1, bool>::value) {
            return std::array{'b'};
        } else if constexpr (std::is_same<T1, int16_t>::value) {
            return std::array{'n'};
        } else if constexpr (std::is_same<T1, uint16_t>::value) {
            return std::array{'q'};
        } else if constexpr (std::is_same<T1, int32_t>::value) {
            return std::array{'i'};
        } else if constexpr (std::is_same<T1, uint32_t>::value) {
            return std::array{'u'};
        } else if constexpr (std::is_same<T1, int64_t>::value) {
            return std::array{'x'};
        } else if constexpr (std::is_same<T1, uint64_t>::value) {
            return std::array{'t'};
        } else if constexpr (std::is_same<T1, double>::value) {
            return std::array{'d'};
            // todo: UNIX_FD
            // } else if constexpr () {
            //     return std::array{'h'};
        } else if constexpr (std::is_same<T1, std::string>::value) {
            return std::array{'s'};
        } else if constexpr (std::is_same<T1, object_path>::value) {
            return std::array{'o'};
        } else if constexpr (std::is_same<T1, dbusx::signature>::value) {
            return std::array{'g'};
        } else if constexpr (is_vector<T1>::value) {
            // array
            return concat(std::array{'a'}, type<typename T1::value_type>::signature);
        } else if constexpr (is_unordered_map<T1>::value) {
            // dict
            return concat(std::array{'{'},
                          type<typename T1::key_type>::signature,
                          type<typename T1::mapped_type>::signature,
                          std::array{'}'});
        } else if constexpr (is_tuple<T1>::value) {
            // struct
            // top-level cv-qualifiers and reference removed
            return tuple_type<T1>::signature;
        } else if constexpr (std::is_same<T1, std::any>::value) {
            return std::array{'v'};
        } else {
            static_assert(always_false_v<T1>, "unsupported type");
        }
    });

    static constexpr auto signature_nt = concat(signature, std::array{'\0'});
};

/*!
  @brief Get D-Bus signature string from C++ types
 */
template <typename... T>
struct types {
    static constexpr auto signature = concat(type<T>::signature...);
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
