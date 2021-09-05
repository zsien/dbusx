#include "dbusx/message.h"

#include <fcntl.h>
#include <unistd.h>

#include "message_private.h"
#include "dbusx/type.h"
#include "dbusx/error.h"

using namespace dbusx;

message::message()
    : d_ptr_(std::make_unique<message_private>()) {
}

message::message(std::unique_ptr<message_private> &&ptr)
    : d_ptr_(std::move(ptr)) {
}

message::~message() = default;

message::message(message &&r) noexcept = default;
message &message::operator=(message &&r) noexcept = default;

message message::create_return() const {
    message msg;
    sd_bus_message_new_method_return(d_ptr_->message_, &msg.d_ptr_->message_);

    return msg;
}

message message::create_error(const error &err) const {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#endif
    auto e = SD_BUS_ERROR_MAKE_CONST(err.name_.c_str(), err.message_.c_str());
#ifdef __clang__
#pragma clang diagnostic pop
#endif

    message msg;
    sd_bus_message_new_method_error(d_ptr_->message_, &msg.d_ptr_->message_, &e);

    return msg;
}

std::string message::get_destination() const {
    return sd_bus_message_get_destination(d_ptr_->message_);
}

std::string message::get_path() const {
    return sd_bus_message_get_path(d_ptr_->message_);
}

std::string message::get_interface() const {
    return sd_bus_message_get_interface(d_ptr_->message_);
}

std::string message::get_member() const {
    return sd_bus_message_get_member(d_ptr_->message_);
}

std::string message::get_sender() const {
    return sd_bus_message_get_sender(d_ptr_->message_);
}

message::type message::get_type() const {
    uint8_t t;
    sd_bus_message_get_type(d_ptr_->message_, &t);

    switch (t) {
    case SD_BUS_MESSAGE_METHOD_CALL:
        return type::METHOD_CALL;
    case SD_BUS_MESSAGE_METHOD_RETURN:
        return type::METHOD_RETURN;
    case SD_BUS_MESSAGE_METHOD_ERROR:
        return type::METHOD_ERROR;
    case SD_BUS_MESSAGE_SIGNAL:
        return type::SIGNAL;
    }

    return type::METHOD_CALL;
}

char message::read_byte() const {
    char i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<char>::signature_nt.data(), &i);
    return i;
}

bool message::read_bool() const {
    int i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<bool>::signature_nt.data(), &i);
    return i;
}

int16_t message::read_int16() const {
    int16_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<int16_t>::signature_nt.data(), &i);
    return i;
}

uint16_t message::read_uint16() const {
    uint16_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<uint16_t>::signature_nt.data(), &i);
    return i;
}

int32_t message::read_int32() const {
    int32_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<int32_t>::signature_nt.data(), &i);
    return i;
}

uint32_t message::read_uint32() const {
    uint32_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<uint32_t>::signature_nt.data(), &i);
    return i;
}

int64_t message::read_int64() const {
    int64_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<int64_t>::signature_nt.data(), &i);
    return i;
}

uint64_t message::read_uint64() const {
    uint64_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<uint64_t>::signature_nt.data(), &i);
    return i;
}

double message::read_double() const {
    double i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<double>::signature_nt.data(), &i);
    return i;
}

std::string message::read_string() const {
    char *s;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<std::string>::signature_nt.data(), &s);
    return s;
}

object_path message::read_object_path() const {
    char *s;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<object_path>::signature_nt.data(), &s);
    return s;
}

signature message::read_signature() const {
    char *s;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<signature>::signature_nt.data(), &s);
    return s;
}

int message::read_fd() const {
    int fd;
    sd_bus_message_read(d_ptr_->message_, "h", &fd);
    return fcntl(fd, F_DUPFD_CLOEXEC, 3);
}

bool message::c_append(const char *signature, ...) {
    va_list ap;
    va_start(ap, signature);
    int r = sd_bus_message_appendv(d_ptr_->message_, signature, ap);
    va_end(ap);

    return r >= 0;
}

bool message::send() {
    auto *bus = sd_bus_message_get_bus(d_ptr_->message_);
    return sd_bus_send(bus, d_ptr_->message_, 0) >= 0;
}
