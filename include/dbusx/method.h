#ifndef DBUSX_METHOD_H
#define DBUSX_METHOD_H

#include <functional>
#include <string_view>
#include <tuple>

#include "interface.h"
#include "message.h"
#include "signature.h"
#include "vtable.h"

namespace dbusx {

template <typename T>
static T set_param(message &m) {
    using Type = std::remove_cvref_t<T>;

    if constexpr (std::is_same<Type, char>::value) {
        return m.read_byte();
    } else if constexpr (std::is_same<Type, bool>::value) {
        return m.read_bool();
    } else if constexpr (std::is_same<Type, int16_t>::value) {
        return m.read_int16();
    } else if constexpr (std::is_same<Type, uint16_t>::value) {
        return m.read_uint16();
    } else if constexpr (std::is_same<Type, int32_t>::value) {
        return m.read_int32();
    } else if constexpr (std::is_same<Type, uint32_t>::value) {
        return m.read_uint32();
    } else if constexpr (std::is_same<Type, int64_t>::value) {
        return m.read_int64();
    } else if constexpr (std::is_same<Type, uint64_t>::value) {
        return m.read_uint64();
    } else if constexpr (std::is_same<Type, double>::value) {
        return m.read_double();
        // todo: UNIX_FD
        // } else if constexpr () {
        //     return std::array{'h'};
    } else if constexpr (std::is_same<Type, std::string>::value) {
        return m.read_string();
        //} else if constexpr (std::is_same<Type, dbus_object_path>::value) {
        //    return std::array{'o'};
        //} else if constexpr (std::is_same<Type, dbus_signature>::value) {
        //    return std::array{'g'};
    } else {
        static_assert(always_false_v<Type>, "unsupported type");
    }
}

template <auto F>
struct method;
template <typename C, typename OUT, typename... IN, OUT (C::*F)(/*Context,*/ IN...)>
struct method<F> {
    static constexpr auto in_cnt = sizeof...(IN);

    template <int N>
    using in_type = std::tuple_element_t<N, std::tuple<IN...>>;

    // using out_type = OUT;

    // std::array<std::string_view>{"i", "t", "as"}
    static constexpr auto in_arr = std::invoke([] {
        return std::array{std::string_view(signature<IN>.cbegin(), signature<IN>.size())...};
    });

    // std::string_view{"as"}
    static constexpr auto out_str = std::invoke(
        [] { return std::string_view(signature<OUT>.cbegin(), signature<OUT>.size()); });

    // std::array{'i', 't', 'a', 's', '\0'}
    static constexpr auto ins =
        std::invoke([] { return concat(signature<IN>..., std::array{'\0'}); });

    // std::array{'a', 's', '\0'}
    static constexpr auto out =
        std::invoke([] { return concat(signature<OUT>, std::array{'\0'}); });

    static vtable::method vtable() {
        // std::vector<std::string> in_signatures;
        // in_signatures.reserve(in_cnt);
        // for (const auto &i : in_arr) {
        //    in_signatures.emplace_back(i);
        //}

        // std::vector<std::string> out_signatures;
        // out_signatures.reserve(out_cnt);
        // for (const auto &i : out_arr) {
        //    out_signatures.emplace_back(i);
        //}

        return {
            .in_signatures = signature_nt<IN...>.data(),
            .in_names = {},
            .out_signatures = signature_nt<OUT>.data(),
            .out_names = {},
            .invoke = &method::invoke,
            .flags = 0,
        };
    }

    static void invoke(interface *o, const message &m) {
        C *obj = reinterpret_cast<C *>(o);
        OUT r = (obj->*F)(m.read<IN>()...);
        // TODO: out

        message ret = m.create_return();
        ret.append(r);
        bool a = ret.send();
        (void)a;
        // m.reply(r);
    }
};

template <auto F>
struct method_multi_return;
template <typename C,
          typename Sender,
          typename... OUT,
          typename... IN,
          std::tuple<OUT...> (C::*F)(Sender, IN...)>
struct method_multi_return<F> {
    static constexpr auto in_cnt = sizeof...(IN);
    static constexpr auto out_cnt = sizeof...(OUT);

    template <int N>
    using in_type = std::tuple_element_t<N, std::tuple<IN...>>;

    // using out_type = OUT;

    // std::array<std::string_view>{"i", "t", "as"}
    static constexpr auto in_arr = method<F>::in_arr;

    // std::array<std::string_view>{"i", "t", "as"}
    static constexpr auto out_arr = std::invoke([] {
        return std::array{std::string_view(signature<OUT>.cbegin(), signature<OUT>.size())...};
    });

    // std::array{'i', 't', 'a', 's', '\0'}
    static constexpr auto ins = method<F>::ins;

    // std::array{'i', 't', 'a', 's', '\0'}
    static constexpr auto outs =
        std::invoke([] { return concat(signature<OUT>..., std::array{'\0'}); });

    static vtable::method vtable() {
        // std::vector<std::string> in_signatures;
        // in_signatures.reserve(in_cnt);
        // for (const auto &i : in_arr) {
        //    in_signatures.emplace_back(i);
        //}

        // std::vector<std::string> out_signatures;
        // out_signatures.reserve(out_cnt);
        // for (const auto &i : out_arr) {
        //    out_signatures.emplace_back(i);
        //}

        return {
            .in_signatures = ins.data(),
            .out_signatures = outs.data(),
        };
    }

    static void invoke(interface &o, message &m) {
        C *obj = reinterpret_cast<C *>(o);
        auto r = obj->F(get_param<IN>(m)...);
        // TODO: out
    }
};

} // namespace dbusx

#endif // !DBUSX_METHOD_H
