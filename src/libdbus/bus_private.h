#ifndef DBUSX_LIBDBUS_DBUSPRIVATE_H
#define DBUSX_LIBDBUS_DBUSPRIVATE_H

#include <dbus/dbus.h>

namespace dbusx {

class bus_private {
    friend class bus;

public:
    bus_private();
    ~bus_private();

private:
    DBusError err_;
    DBusConnection *conn_;
};

} // namespace dbusx

#endif // !DBUSX_LIBDBUS_DBUSPRIVATE_H
