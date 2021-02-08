#include "dbusx/bus.h"

#include <stdexcept>
#include <memory>

#include <systemd/sd-bus.h>

#include "bus_private.h"
#include "dbusx/interface.h"
#include "dbusx/message.h"
#include "message_private.h"

using namespace dbusx;

bus::bus(type type)
    : d_ptr_(std::make_unique<bus_private>()) {
    decltype(&sd_bus_open) f;

    switch (type) {
    case type::USER:
        f = &sd_bus_open_user;
        break;
    case type::SYSTEM:
        f = &sd_bus_open_system;
        break;
    case type::STARTER:
        f = &sd_bus_open;
        break;
    }

    int ret = f(&d_ptr_->conn_);
    if (ret < 0) {
        throw std::runtime_error(std::string("Failed to connect to dbus: ") + strerror(-ret));
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
bool bus::export_interface(const std::string &path, const std::string &iface, interface *obj) {
    auto &exported_interfaces = d_ptr_->exported_[path];

    if (exported_interfaces.find(iface) != exported_interfaces.end()) {
        // this interface exported
        return false;
    }

    auto exported = obj->exported();

    auto d = std::make_unique<data>();

    d->ud.reserve(exported.methods.size() +      // methods
                  exported.properties.size() * 2 // properties getter + setter
    );

    d->vtable.reserve(1 +                          // start
                      exported.methods.size() +    // methods
                      exported.properties.size() + // properties
                      exported.signals.size() +    // signals
                      1                            // end
    );

    d->vtable.push_back(SD_BUS_VTABLE_START(0));

    for (const auto &i : exported.methods) {
        auto current_offset = d->ud.size() * sizeof(userdata);

        d->ud.emplace_back(userdata{
            .i = obj,
            .invoker = i.second.invoker,
        });

        d->vtable.push_back(SD_BUS_METHOD_WITH_OFFSET(i.first.c_str(),
                                                      i.second.in_signatures,
                                                      i.second.out_signatures,
                                                      &bus_private::on_method_call,
                                                      current_offset,
                                                      SD_BUS_VTABLE_UNPRIVILEGED));
    }

    // for (const auto &i : exported.properties) {
    // }

    // for (const auto &i : exported.signals) {
    // }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    d->vtable.push_back(SD_BUS_VTABLE_END);
#pragma GCC diagnostic pop

    int ret = sd_bus_add_object_vtable(d_ptr_->conn_,
                                       nullptr,
                                       path.c_str(),
                                       iface.c_str(),
                                       d->vtable.data(),
                                       d->ud.data());

    exported_interfaces.emplace(iface, std::move(d));

    // TODO: 错误处理
    return ret < 0;
}

// TODO: 优化、错误处理
void bus::start() {
    int ret;
    for (;;) {
        ret = sd_bus_process(d_ptr_->conn_, nullptr);
        if (ret < 0) {
            fprintf(stderr, "Failed to process bus: %s\n", strerror(-ret));
            break;
        }
        if (ret > 0) {
            continue;
        }

        ret = sd_bus_wait(d_ptr_->conn_, UINT64_MAX);
        if (ret < 0) {
            fprintf(stderr, "Failed to wait on bus: %s\n", strerror(-ret));
            break;
        }
    }
}
