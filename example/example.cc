#include "example.h"

#include <dbusx/method.h>
#include <dbusx/property.h>
#include <dbusx/error.h>

example::example() {
}

uint32_t example::string_length(const std::string &str) {
    return str.length();
}

tl::expected<std::string, dbusx::error> example::may_error(bool should) {
    if (should) {
        return tl::make_unexpected(
            dbusx::error("org.freedesktop.DBus.Error.FileNotFound", "error message"));
    }

    return std::string("no error~");
}

std::string example::echo(const std::string &str) {
    return str;
}

std::string example::get_read_only_propery() {
    return "read only !!!";
}

tl::expected<std::string, dbusx::error> example::get_read_only_propery_e() {
    return tl::make_unexpected(
        dbusx::error("org.freedesktop.DBus.Error.FileNotFound", "error message"));
}

std::string example::get_writable_propery() {
    return writable_property_;
}

void example::set_writable_propery(const std::string &str) {
    writable_property_ = str;
}

std::string example::interface_name() {
    return "cn.zsien.dbusx.example.control";
}

dbusx::vtable::vtable example::exported() {
    return {
        .methods =
            {
                {"StringLength", dbusx::method<&example::string_length>::get_vtable()},
                {"MayError", dbusx::method<&example::may_error>::get_vtable()},
                {"Echo", dbusx::method<&example::echo>::get_vtable()},
            },
        .properties =
            {
                {"ReadOnlyProperty",
                 dbusx::property<&example::get_read_only_propery>::get_vtable()},
                {"ReadOnlyPropertyE",
                 dbusx::property<&example::get_read_only_propery_e>::get_vtable()},
                {"WritableProperty",
                 dbusx::property<&example::get_writable_propery,
                                 &example::set_writable_propery>::get_vtable()},
            },
        .signals = {},
    };
}
