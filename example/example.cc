#include "example.h"

#include <dbusx/method.h>
#include <dbusx/property.h>
#include <dbusx/error.h>

example::example() {
}

uint32_t example::string_length(const std::string &str) {
    return str.length();
}

std::string example::echo(const std::string &str) {
    return str;
}

tl::expected<std::string, dbusx::error> example::may_error(bool error) {
    if (error) {
        return tl::make_unexpected(
            dbusx::error("org.freedesktop.DBus.Error.FileNotFound", "error message"));
    }

    return {"no error ~"};
}

dbusx::object_path example::get_path() {
    return dbusx::object_path("/cn/zsien/dbusx/example/path");
}

void example::no_return() {
}

tl::expected<void, dbusx::error> example::no_return_or_error(bool error) {
    if (error) {
        return tl::make_unexpected(
            dbusx::error("org.freedesktop.DBus.Error.FileNotFound", "error message"));
    }

    return {};
}

std::string example::multi_params([[maybe_unused]] const std::string &str1,
                                  const std::string &str2) {
    return str2;
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
                {"Echo", dbusx::method<&example::echo>::get_vtable()},
                {"MayError", dbusx::method<&example::may_error>::get_vtable()},
                {"GetPath", dbusx::method<&example::get_path>::get_vtable()},
                {"NoReturn", dbusx::method<&example::no_return>::get_vtable()},
                {"NoReturnOrError", dbusx::method<&example::no_return_or_error>::get_vtable()},
                {"MultiParams", dbusx::method<&example::multi_params>::get_vtable()},
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
