#ifndef DBUSX_VTABLE_H
#define DBUSX_VTABLE_H

#include <string>
#include <variant>
#include <vector>

namespace dbusx {

namespace vtable {

enum class type {property, method, signal};

struct property {
    std::string signature;
};

struct method {
    const char *in_signatures;
    std::vector<std::string> in_names;
    const char *out_signatures;
    std::vector<std::string> out_names;
};

struct signal {
    std::string member;
    std::vector<std::string> signatures;
    std::vector<std::string> names;
};

struct vtable {
    type type;
    uint64_t flags;
    std::string member;
    std::variant<property, method, signal> x;
};

} // namespace vtable

} // namespace dbusx

#endif // !DBUSX_VTABLE_H
