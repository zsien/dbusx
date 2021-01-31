#include "dbusx/message.h"

#include <fcntl.h>
#include <unistd.h>

#include "message_private.h"

using namespace dbusx;

message::message(std::unique_ptr<message_private> &&ptr)
    : d_ptr_(std::move(ptr)) {
}

std::string message::get_string() {
    char *s;
    sd_bus_message_read(d_ptr_->message_, "s", &s);

    return s;
}

int message::get_fd() {
    int fd;
    sd_bus_message_read(d_ptr_->message_, "h", &fd);
    return fcntl(fd, F_DUPFD_CLOEXEC, 3);
}
