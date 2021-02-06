#include "bus_private.h"

#include "dbusx/message.h"
#include "message_private.h"

using namespace dbusx;

data::data(bus *b, interface *i)
    : b(b)
    , i(i) {
}

int bus_private::on_method_call(sd_bus_message *m, void *userdata, [[maybe_unused]] sd_bus_error *error) {
    auto *d = reinterpret_cast<data *>(userdata);
    d->b->on_method_call(d, message(std::make_unique<message_private>(m)));

    return 0;
}
