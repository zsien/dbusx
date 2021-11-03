#ifndef DBUSX_VTABLE_H
#define DBUSX_VTABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "error.h"

namespace dbusx {

class interface;
class message;

namespace vtable {

using method_invoker = void (*)(interface *, const message &);
using property_getter = std::optional<error> (*)(interface *, message &);
using property_setter = std::optional<error> (*)(interface *, message &);

enum class type { property, method, signal };

struct method {
    const char *in_signatures;
    std::vector<std::string> in_names;
    const char *out_signatures;
    std::vector<std::string> out_names;
    method_invoker invoker;
    uint64_t flags;
};

struct property {
    const char *signature;
    property_getter getter;
    property_setter setter;
    uint64_t flags;
};

struct signal {
    std::vector<std::string> signatures;
    std::vector<std::string> names;
    uint64_t flags;
};

struct vtable {
    std::unordered_map<const char *, method> methods;
    std::unordered_map<const char *, property> properties;
    std::unordered_map<std::string, signal> signals;
};

} // namespace vtable

} // namespace dbusx

#endif // !DBUSX_VTABLE_H
