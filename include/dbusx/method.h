#ifndef DBUSX_METHOD_H
#define DBUSX_METHOD_H

#include <functional>
#include <string_view>
#include <tuple>

#include "signature.h"
#include "vtable.h"

namespace dbusx {

template <auto F>
struct method;
template <typename C,
          typename Sender,
          typename... OUT,
          typename... IN,
          std::tuple<OUT...> (C::*F)(Sender, IN...)>
struct method<F> {
    static constexpr auto in_cnt = sizeof...(IN);
    static constexpr auto out_cnt = sizeof...(OUT);

    template <int N>
    using in_type = std::tuple_element_t<N, std::tuple<IN...>>;

    // using out_type = R;

    // std::array<std::string_view>{"i", "t", "as"}
    static constexpr auto in_arr = std::invoke([] {
        return std::array{std::string_view(signature<IN>.cbegin(),
                                           signature<IN>.size())...};
    });

    // std::array<std::string_view>{"i", "t", "as"}
    static constexpr auto out_arr = std::invoke([] {
        return std::array{std::string_view(signature<OUT>.cbegin(),
                                           signature<OUT>.size())...};
    });

    // std::array{'i', 't', 'a', 's', '\0'}
    static constexpr auto ins = std::invoke(
            [] { return concat(signature<IN>..., std::array{'\0'}); });

    // std::array{'i', 't', 'a', 's', '\0'}
    static constexpr auto outs = std::invoke(
            [] { return concat(signature<OUT>..., std::array{'\0'}); });

    static vtable::vtable vtable(const std::string &member) {
        // std::vector<std::string> in_signatures;
        // in_signatures.reserve(in_cnt);
        // for (const auto &i : in_arr) {
        //    in_signatures.emplace_back(i);
        //}

        std::vector<std::string> out_signatures;
        out_signatures.reserve(out_cnt);
        for (const auto &i : out_arr) {
            out_signatures.emplace_back(i);
        }

        return vtable::vtable{
                .type = vtable::type::method,
                .member = member,
                .x =
                        vtable::method{
                                .in_signatures = ins.data(),
                                .out_signatures = outs.data(),
                        },
        };
    }
};

} // namespace dbusx

#endif // !DBUSX_METHOD_H
