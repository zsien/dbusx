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
constexpr auto signature_single = std::invoke([] {
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
    } else if constexpr (is_vector<Type>::value) {
        // array
        return concat(std::array{'a'}, signature_single<typename Type::value_type>);
    } else if constexpr (is_unordered_map<Type>::value) {
        // dict
        return concat(std::array{'{'},
                      signature_single<typename Type::key_type>,
                      signature_single<typename Type::mapped_type>,
                      std::array{'}'});
    } else if constexpr (is_tuple<Type>::value) {
        // struct
        // top-level cv-qualifiers and reference removed
        return signature_single<Type>;
    } else if constexpr (std::is_same<Type, std::any>::value) {
        return std::array{'v'};
    } else {
        static_assert(always_false_v<Type>, "unsupported type");
    }
});

// struct
template <typename... T>
constexpr auto signature_single<std::tuple<T...>> = concat(std::array{'('}, signature_single<T>..., std::array{')'});

template <typename... T>
constexpr auto signature = concat(signature_single<T>...);

// signature with null-terminated
template <typename T>
constexpr auto signature_nt = concat(signature<T>, std::array{'\0'});

template <typename... T>
constexpr auto signature_nt_arr = std::array{signature_nt<T>.data()...};

template <typename T>
auto unwrap_type() {
}

} // namespace dbusx

#endif // !DBUSX_SIGNATURE_H
