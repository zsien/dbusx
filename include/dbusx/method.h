#ifndef DBUSX_METHOD_H
#define DBUSX_METHOD_H

#include <functional>
#include <string_view>
#include <tuple>

#include "interface.h"
#include "message.h"
#include "signature.h"
#include "vtable.h"
#include "error.h"

namespace dbusx {

template <auto F>
struct method;
template <typename C, typename OUT, typename... IN, OUT (C::*F)(/*Context,*/ IN...)>
struct method<F> {
    static constexpr auto in_cnt = sizeof...(IN);

    template <int N>
    using in_type = std::tuple_element_t<N, std::tuple<IN...>>;

    // using out_type = OUT;

    // const char *const *{"i", "t", "as"}
    static constexpr auto in_arr = signature_nt_arr<IN...>.data();

    // std::string_view{"as"}
    static constexpr auto out_str =
        std::string_view(signature<OUT>.cbegin(), signature<OUT>.size());

    static vtable::method get_vtable() {
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
            .invoker = &method::invoke,
            .flags = 0,
        };
    }

    static void invoke(interface *o, const message &m) {
        try {
            C *obj = reinterpret_cast<C *>(o);
            OUT r = (obj->*F)(m.read<IN>()...);
            // TODO: out
            (void)r;

            message ret = m.create_return();
            ret.append(r);
            bool a = ret.send();
            (void)a;
            // m.reply(r);
        } catch (const dbusx::error &e) {
            message ret = m.create_error(e);
            bool a = ret.send();
            (void)a;
        }
    }
};

} // namespace dbusx

#endif // !DBUSX_METHOD_H
