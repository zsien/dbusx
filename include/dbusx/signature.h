#ifndef DBUSX_SIGNATURE_H
#define DBUSX_SIGNATURE_H

#include <any>
#include <array>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dbusx/utils.h"

namespace dbusx {

template <typename T>
constexpr auto signature_basic = std::invoke([] {
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
        // todo: UNIX_FD
        // } else if constexpr () {
        //     return std::array{'h'};
    } else if constexpr (std::is_same<Type, std::string>::value) {
        return std::array{'s'};
        //} else if constexpr (std::is_same<Type, dbus_object_path>::value) {
        //    return std::array{'o'};
        //} else if constexpr (std::is_same<Type, dbus_signature>::value) {
        //    return std::array{'g'};
    } else if constexpr (std::is_same<Type, std::any>::value) {
        return std::array{'v'};
    } else {
        static_assert(always_false_v<Type>, "unsupported type");
    }
});

// array
template <typename T>
constexpr auto signature_basic<std::vector<T>> = std::invoke([] {
    return concat(std::array{'a'}, signature_basic<T>);
});

// dict
template <typename F, typename S>
constexpr auto signature_basic<std::unordered_map<F, S>> = std::invoke([] {
    return concat(std::array{'{'}, signature_basic<F>, signature_basic<S>, std::array{'}'});
});

// struct
template <typename... T>
constexpr auto signature_basic<std::tuple<T...>> = std::invoke([] {
    return concat(std::array{'('}, signature_basic<T>..., std::array{')'});
});

template <typename... T>
constexpr auto signature = std::invoke([] { return concat(signature_basic<T>...); });

// signature with null-terminated
template <typename T>
constexpr auto signature_nt = std::invoke([] { return concat(signature<T>, std::array{'\0'}); });

template <typename T>
auto unwrap_type() {
}

} // namespace dbusx

#endif // !DBUSX_SIGNATURE_H
