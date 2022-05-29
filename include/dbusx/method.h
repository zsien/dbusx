#ifndef DBUSX_METHOD_H
#define DBUSX_METHOD_H

#include <functional>
#include <string_view>
#include <tuple>
#include <functional>

#include "interface.h"
#include "message.h"
#include "type.h"
#include "vtable.h"
#include "error.h"

namespace dbusx {

template <auto F>
struct method;

/*!
  @brief A wrapper to generate a vtable item of method

  @tparam F method pointer, it should be a member function of current interface
 */
template <typename C, typename OUT, typename... IN, OUT (C::*F)(/*Context,*/ IN...)>
struct method<F> {
    static vtable::method get_vtable() {
        return {
            .in_signatures = types<IN...>::signature_nt.data(),
            .in_names = {},
            .out_signatures = type<typename return_type<OUT>::type>::signature_nt.data(),
            .out_names = {},
            .invoker = &method::invoke,
            .flags = 0,
        };
    }

    static void invoke(interface *o, const message &m) {
        C *obj = reinterpret_cast<C *>(o);

        if constexpr (std::is_void_v<OUT>) {
            // no return
            (obj->*F)(m.read<IN>()...);
            message ret = m.create_return();
            bool a = ret.send();
            (void)a;
        } else {
            OUT r = (obj->*F)(m.read<IN>()...);
            if constexpr (tl::detail::is_expected<OUT>::value) {
                // return tl::expected
                if (r) {
                    message ret = m.create_return();
                    if constexpr (!std::is_void_v<typename OUT::value_type>) {
                        ret.append(r.value());
                    } // else return tl::expected<void, dbusx::error>
                    bool a = ret.send();
                    (void)a;
                } else {
                    message ret = m.create_error(r.error());
                    bool a = ret.send();
                    (void)a;
                }
            } else {
                // return value with no error
                message ret = m.create_return();
                ret.append(r);
                bool a = ret.send();
                (void)a;
            }
        }
    }
};

} // namespace dbusx

#endif // !DBUSX_METHOD_H
