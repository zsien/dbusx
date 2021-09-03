#ifndef DBUSX_SDBUS_DBUSPRIVATE_H
#define DBUSX_SDBUS_DBUSPRIVATE_H

#include "dbusx/bus.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

#include <systemd/sd-bus.h>

#include "dbusx/method.h"

namespace dbusx {

struct __attribute__((visibility("hidden"))) userdata {
    interface *i;
    void (*invoker)(interface *, const message &);
};

struct __attribute__((visibility("hidden"))) data {
    std::vector<userdata> ud;
    std::vector<sd_bus_vtable> vtable;       // keep vtable in memory
    std::unordered_set<std::string> strings; // keep strings in memory
};

class __attribute__((visibility("hidden"))) bus_private {
    friend class bus;

private:
    sd_bus *conn_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<data>>>
        exported_;

    static int on_method_call(sd_bus_message *m, void *userdata, sd_bus_error *error);
    static int on_property_get(sd_bus *bus,
                               const char *path,
                               const char *interface,
                               const char *property,
                               sd_bus_message *reply,
                               void *userdata,
                               sd_bus_error *ret_error);
};

} // namespace dbusx

#endif // !DBUSX_SDBUS_DBUSPRIVATE_H
