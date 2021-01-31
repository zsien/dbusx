#include "dbusx/bus.h"

#include <stdexcept>

#include <systemd/sd-bus.h>

#include "bus_private.h"
#include "dbusx/interface.h"
#include "dbusx/message.h"
#include "message_private.h"

using namespace dbusx;

#define check(x)                                                               \
    ({                                                                         \
        int r = x;                                                             \
        errno = r < 0 ? -r : 0;                                                \
        printf(#x ": %m\n");                                                   \
        if (r < 0) return false;                                               \
    })

static int
method_callback(sd_bus_message *m, void *userdata, sd_bus_error *error) {
    message msg(std::make_unique<message_private>(m));

    printf("Got called with userdata=%p\n", userdata);
    error->message = "gogogo";
    error->name = "err name";
    error->_need_free = static_cast<int>(false);
    return 1;
}

bus::bus(type type)
    : d_ptr_(std::make_unique<bus_private>()) {
    decltype(&sd_bus_open) f;

    switch (type) {
    case type::user:
        f = &sd_bus_open_user;
        break;
    case type::system:
        f = &sd_bus_open_system;
        break;
    case type::starter:
        f = &sd_bus_open;
        break;
    }

    int ret = f(&d_ptr_->conn_);
    if (ret < 0) {
        throw std::runtime_error(std::string("Failed to connect to dbus: ") +
                                 strerror(ret));
    }
}

bus::~bus() {
    sd_bus_flush_close_unref(d_ptr_->conn_);
}

bool bus::request_name(const std::string &name) {
    return sd_bus_request_name(d_ptr_->conn_, name.c_str(), 0) >= 0;
}

bool bus::release_name(const std::string &name) {
    return sd_bus_release_name(d_ptr_->conn_, name.c_str()) >= 0;
}

// TODO: 优化
bool bus::export_interface(const std::string &path,
                           const std::string &iface,
                           interface *obj) {
    auto exported = obj->exported();

    auto *vtable = new std::vector<sd_bus_vtable>;
    vtable->reserve(1 + exported.size() + 1);

    vtable->push_back(SD_BUS_VTABLE_START(0));

    for (const auto &i : exported) {
        switch (i.type) {
        case vtable::type::method: {
            const auto &m = std::get<vtable::method>(i.x);
            vtable->push_back(SD_BUS_METHOD(i.member.c_str(),
                                            m.in_signatures,
                                            m.out_signatures,
                                            method_callback,
                                            0));
        } break;
        default:
            break;
        }
    }

    vtable->push_back(SD_BUS_VTABLE_END);

    int ret = sd_bus_add_object_vtable(d_ptr_->conn_,
                                       nullptr,
                                       path.c_str(),
                                       iface.c_str(),
                                       vtable->data(),
                                       obj);

    // TODO: 错误处理
    return ret < 0;
}

// TODO: 优化、错误处理
void bus::start() {
    for (;;) {
        sd_bus_wait(d_ptr_->conn_, UINT64_MAX);
        sd_bus_process(d_ptr_->conn_, NULL);
    }
}
