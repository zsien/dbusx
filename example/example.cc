#include "example.h"

#include <dbusx/method.h>
#include <dbusx/property.h>
#include <dbusx/error.h>

example::example() {
}

uint32_t example::string_length(const std::string &str) {
    return str.length();
}

int32_t example::always_error(int32_t) {
    throw dbusx::error("org.freedesktop.DBus.Error.FileNotFound", "error message");
}

std::string example::echo(const std::string &str) {
    return str;
}

std::string example::interface_name() {
    return "cn.zsien.dbusx.example.control";
}

std::string example::get_read_only_propery() {
    return "read only !!!";
}

std::string example::get_read_only_propery_error() {
    throw dbusx::error("org.freedesktop.DBus.Error.FileNotFound", "can't read");
}

dbusx::vtable::vtable example::exported() {
    return {
        .methods =
            {
                {"stringLength", dbusx::method<&example::string_length>::get_vtable()},
                {"alwaysError", dbusx::method<&example::always_error>::get_vtable()},
                {"echo", dbusx::method<&example::echo>::get_vtable()},
            },
        .properties = {
            {
                {"readOnlyProperty", dbusx::property<&example::get_read_only_propery>::get_vtable()},
                {"readOnlyPropertyError", dbusx::property<&example::get_read_only_propery_error>::get_vtable()},
            },
        },
        .signals = {},
    };
}
