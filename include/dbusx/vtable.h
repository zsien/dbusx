#ifndef DBUSX_VTABLE_H
#define DBUSX_VTABLE_H

#include <string>
#include <vector>
#include <unordered_map>

namespace dbusx {

class interface;
class message;

namespace vtable {

using method_invoker = void (*)(interface *, const message &);

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
    uint64_t flags;
};

struct signal {
    std::vector<std::string> signatures;
    std::vector<std::string> names;
    uint64_t flags;
};

struct vtable {
    std::unordered_map<std::string, method> methods;
    std::unordered_map<std::string, property> properties;
    std::unordered_map<std::string, signal> signals;
};

} // namespace vtable

} // namespace dbusx

#endif // !DBUSX_VTABLE_H
