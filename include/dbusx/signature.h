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
constexpr auto signature = std::invoke([] {
    using Type = std::remove_cvref_t<T>;

    if constexpr (std::is_same_v<Type, char>) {
        return std::array{'y'};
    } else if constexpr (std::is_same_v<Type, bool>) {
        return std::array{'b'};
    } else if constexpr (std::is_same_v<Type, int16_t>) {
        return std::array{'n'};
    } else if constexpr (std::is_same_v<Type, uint16_t>) {
        return std::array{'q'};
    } else if constexpr (std::is_same_v<Type, int32_t>) {
        return std::array{'i'};
    } else if constexpr (std::is_same_v<Type, uint32_t>) {
        return std::array{'u'};
    } else if constexpr (std::is_same_v<Type, int64_t>) {
        return std::array{'x'};
    } else if constexpr (std::is_same_v<Type, uint64_t>) {
        return std::array{'t'};
    } else if constexpr (std::is_same_v<Type, double>) {
        return std::array{'d'};
        // todo: UNIX_FD
        // } else if constexpr () {
        //     return std::array{'h'};
    } else if constexpr (std::is_same_v<Type, std::string>) {
        return std::array{'s'};
        //} else if constexpr (std::is_same_v<Type, dbus_object_path>) {
        //    return std::array{'o'};
        //} else if constexpr (std::is_same_v<Type, dbus_signature>) {
        //    return std::array{'g'};
    } else if constexpr (std::is_same_v<Type, std::any>) {
        return std::array{'v'};
    } else {
        static_assert(always_false_v<Type>, "unsupported type");
    }
});

// array
template <typename T>
constexpr auto signature<std::vector<T>> = std::invoke([] {
    return concat(std::array{'a'}, signature<T>);
});

// dict
template <typename F, typename S>
constexpr auto signature<std::unordered_map<F, S>> = std::invoke([] {
    return concat(std::array{'{'}, signature<F>, signature<S>, std::array{'}'});
});

// struct
template <typename... T>
constexpr auto signature<std::tuple<T...>> = std::invoke([] {
    return concat(std::array{'('}, signature<T>..., std::array{')'});
});

} // namespace dbusx

#endif // !DBUSX_SIGNATURE_H
