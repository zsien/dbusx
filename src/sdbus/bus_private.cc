#include "bus_private.h"

#include <thread>
#include <chrono>

#include "dbusx/message.h"
#include "message_private.h"

using namespace dbusx;

int bus_private::on_method_call(sd_bus_message *m,
                                void *userdata,
                                [[maybe_unused]] sd_bus_error *error) {
    message msg(std::make_unique<message_private>(m));

    try {
        auto *d = reinterpret_cast<dbusx::userdata *>(userdata);
        d->caller.invoke(d->i, msg);
    } catch (const dbusx::error &e) {
        message ret = msg.create_error(e);
        bool a = ret.send();
        (void)a;
    }

    return 0;
}

int bus_private::on_property_get([[maybe_unused]] sd_bus *bus,
                                 [[maybe_unused]] const char *path,
                                 [[maybe_unused]] const char *interface,
                                 [[maybe_unused]] const char *property,
                                 sd_bus_message *reply,
                                 void *userdata,
                                 [[maybe_unused]] sd_bus_error *ret_error) {
    message m(std::make_unique<message_private>(reply));

    try {
        auto *d = reinterpret_cast<dbusx::userdata *>(userdata);
        d->caller.property.get(d->i, m);
    } catch (const dbusx::error &err) {
        ret_error->name = strndup(err.name_.c_str(), err.name_.length());
        ret_error->message = strndup(err.message_.c_str(), err.message_.length());
        ret_error->_need_free = true;
    }

    return 0;
}

int bus_private::on_property_set([[maybe_unused]] sd_bus *bus,
                                 [[maybe_unused]] const char *path,
                                 [[maybe_unused]] const char *interface,
                                 [[maybe_unused]] const char *property,
                                 sd_bus_message *reply,
                                 void *userdata,
                                 [[maybe_unused]] sd_bus_error *ret_error) {
    message m(std::make_unique<message_private>(reply));

    auto *d = reinterpret_cast<dbusx::userdata *>(userdata);
    d->caller.property.set(d->i, m);

    return 0;
}
