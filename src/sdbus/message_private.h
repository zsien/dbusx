#ifndef DBUSX_SDBUS_MESSAGE_PRIVATE_H
#define DBUSX_SDBUS_MESSAGE_PRIVATE_H

#include <systemd/sd-bus.h>

namespace dbusx {

class __attribute__((visibility("hidden"))) message_private {
    friend class message;

public:
    explicit message_private(sd_bus_message *message);

private:
    sd_bus_message *message_;
};

} // namespace dbusx

#endif //! DBUSX_SDBUS_MESSAGE_PRIVATE_H
