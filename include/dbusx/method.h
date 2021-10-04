#ifndef DBUSX_METHOD_H
#define DBUSX_METHOD_H

#include <functional>
#include <string_view>
#include <tuple>

#include "interface.h"
#include "message.h"
#include "type.h"
#include "vtable.h"
#include "error.h"

namespace dbusx {

template <auto F>
struct method;
template <typename C, typename OUT, typename... IN, OUT (C::*F)(/*Context,*/ IN...)>
struct method<F> {
    static vtable::method get_vtable() {
        return {
            .in_signatures = types<IN...>::signature_nt.data(),
            .in_names = {},
            .out_signatures = type<OUT>::signature_nt.data(),
            .out_names = {},
            .invoker = &method::invoke,
            .flags = 0,
        };
    }

    static void invoke(interface *o, const message &m) {
        try {
            C *obj = reinterpret_cast<C *>(o);
            OUT r = (obj->*F)(m.read<IN>()...);

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
