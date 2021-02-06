#ifndef DBUSX_SDBUS_DBUSPRIVATE_H
#define DBUSX_SDBUS_DBUSPRIVATE_H

#include "dbusx/bus.h"

#include <systemd/sd-bus.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

namespace dbusx {

struct data {
    bus *b;
    interface *i;
    std::unordered_map<std::string, void (*)(interface *, const message &)> invokers;
    std::vector<sd_bus_vtable> vtable;       // keep vtable in memory
    std::unordered_set<std::string> strings; // keep strings in memory

    data(bus *b, interface *i);
};

class __attribute__((visibility("hidden"))) bus_private {
    friend class bus;

private:
    sd_bus *conn_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<data>>> ctx_;

    static int on_method_call(sd_bus_message *m, void *userdata, sd_bus_error *error);
};

} // namespace dbusx

#endif // !DBUSX_SDBUS_DBUSPRIVATE_H
