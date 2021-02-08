#include "example.h"

#include <dbusx/method.h>

example::example() {
}

uint32_t example::string_length(const std::string &str) {
    return str.length();
}

std::string example::interface_name() {
    return "cn.zsien.dbusx.example.control";
}

dbusx::vtable::vtable example::exported() {
    return {
        .methods =
            {
                {"stringLength", dbusx::method<&example::string_length>::get_vtable()},
            },
        .properties = {},
        .signals = {},
    };
}
