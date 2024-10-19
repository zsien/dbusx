#include "message_private.h"

using namespace dbusx;

message_private::message_private(sd_bus_message *message)
    : message_(message) {
}

message_private::~message_private() {
    // sd_bus_message_unref(message_);
}
