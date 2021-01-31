#ifndef DBUSX_SDBUS_DBUSPRIVATE_H
#define DBUSX_SDBUS_DBUSPRIVATE_H

#include "dbusx/bus.h"

#include <systemd/sd-bus.h>

namespace dbusx {

class __attribute__((visibility("hidden"))) bus_private {
    friend class bus;

private:
    sd_bus *conn_;
};

} // namespace dbusx

#endif // !DBUSX_SDBUS_DBUSPRIVATE_H
