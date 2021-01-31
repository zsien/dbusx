#include "bus_private.h"

#include <dbus/dbus.h>

using namespace dbusx;

bus_private::bus_private() {
    dbus_error_init(&err_);
}

bus_private::~bus_private() {
    dbus_error_free(&err_);
}
