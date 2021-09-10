#ifndef DBUSX_PROPERTY_H
#define DBUSX_PROPERTY_H

#include "vtable.h"
#include "interface.h"
#include "message.h"
#include "error.h"

namespace dbusx {

template <auto F>
struct property;
template <typename C, typename TYPE, TYPE (C::*GETTER)()>
struct property<GETTER> {
    static vtable::property get_vtable() {
        return {
            .signature = types<TYPE>::signature_nt.data(),
            .getter = get,
            .setter = nullptr,
            .flags = 0,
        };
    }

    static void get(interface *o, message &m) {
        C *obj = reinterpret_cast<C *>(o);
        TYPE r = (obj->*GETTER)();

        m.append(r);
    }
};

} // namespace dbusx

#endif // !DBUSX_PROPERTY_H
