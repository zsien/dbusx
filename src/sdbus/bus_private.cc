#include "bus_private.h"

#include "dbusx/message.h"
#include "message_private.h"

using namespace dbusx;

int bus_private::on_method_call(sd_bus_message *m,
                                void *userdata,
                                [[maybe_unused]] sd_bus_error *error) {
    auto *d = reinterpret_cast<dbusx::userdata *>(userdata);
    d->invoker(d->i, message(std::make_unique<message_private>(m)));

    return 0;
}

/*
int bus_private::on_property_get(sd_bus *bus,
                                 const char *path,
                                 const char *interface,
                                 const char *property,
                                 sd_bus_message *reply,
                                 void *userdata,
                                 sd_bus_error *ret_error) {
}
*/
