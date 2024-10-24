#include "dbusx/message.h"

#include "message_private.h"
#include "dbusx/type.h"
#include "dbusx/error.h"

#include <fcntl.h>
#include <unistd.h>

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
    sd_bus_message_read(d_ptr_->message_, dbusx::type<char>::signature_str.c_str(), &i);
    return i;
}

bool message::read_bool() const {
    int i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<bool>::signature_str.c_str(), &i);
    return i;
}

int16_t message::read_int16() const {
    int16_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<int16_t>::signature_str.c_str(), &i);
    return i;
}

uint16_t message::read_uint16() const {
    uint16_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<uint16_t>::signature_str.c_str(), &i);
    return i;
}

int32_t message::read_int32() const {
    int32_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<int32_t>::signature_str.c_str(), &i);
    return i;
}

uint32_t message::read_uint32() const {
    uint32_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<uint32_t>::signature_str.c_str(), &i);
    return i;
}

int64_t message::read_int64() const {
    int64_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<int64_t>::signature_str.c_str(), &i);
    return i;
}

uint64_t message::read_uint64() const {
    uint64_t i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<uint64_t>::signature_str.c_str(), &i);
    return i;
}

double message::read_double() const {
    double i;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<double>::signature_str.c_str(), &i);
    return i;
}

std::string message::read_string() const {
    char *s;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<std::string>::signature_str.c_str(), &s);
    return s;
}

object_path message::read_object_path() const {
    char *s;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<object_path>::signature_str.c_str(), &s);
    return object_path(s);
}

signature message::read_signature() const {
    char *s;
    sd_bus_message_read(d_ptr_->message_, dbusx::type<signature>::signature_str.c_str(), &s);
    return signature(s);
}

fd message::read_fd() const {
    int f;
    sd_bus_message_read(d_ptr_->message_, "h", &f);
    return fd(fcntl(f, F_DUPFD_CLOEXEC, 3));
}

bool message::append_byte(char y) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<char>::signature_str.c_str(), y);
}

bool message::append_bool(bool b) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<bool>::signature_str.c_str(), b);
}

bool message::append_int16(int16_t n) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<int16_t>::signature_str.c_str(), n);
}

bool message::append_uint16(uint16_t q) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<uint16_t>::signature_str.c_str(), q);
}

bool message::append_int32(int32_t i) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<int32_t>::signature_str.c_str(), i);
}

bool message::append_uint32(uint32_t u) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<uint32_t>::signature_str.c_str(), u);
}

bool message::append_int64(int64_t x) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<int64_t>::signature_str.c_str(), x);
}

bool message::append_uint64(uint64_t t) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<uint64_t>::signature_str.c_str(), t);
}

bool message::append_double(double d) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<double>::signature_str.c_str(), d);
}

bool message::append_fd(fd f) {
    return sd_bus_message_append(d_ptr_->message_, dbusx::type<fd>::signature_str.c_str(), int(f));
}

bool message::append_string(const std::string &s) {
    return sd_bus_message_append(d_ptr_->message_,
                                 dbusx::type<std::string>::signature_str.c_str(),
                                 s.c_str());
}

bool message::append_object_path(const object_path &o) {
    return sd_bus_message_append(d_ptr_->message_,
                                 dbusx::type<object_path>::signature_str.c_str(),
                                 std::string(o).c_str());
}

bool message::append_signature(const signature &g) {
    return sd_bus_message_append(d_ptr_->message_,
                                 dbusx::type<signature>::signature_str.c_str(),
                                 std::string(g).c_str());
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

void message::enter_container(char type, const char *contents) const {
    sd_bus_message_enter_container(d_ptr_->message_, type, contents);
}

void message::exit_container() const {
    sd_bus_message_exit_container(d_ptr_->message_);
}

bool message::at_end(bool current) const {
    return sd_bus_message_at_end(d_ptr_->message_, !current);
}

bool message::open_container(char type, const char *contents) {
    sd_bus_message_open_container(d_ptr_->message_, type, contents);
    return true;
}

bool message::close_container() {
    sd_bus_message_close_container(d_ptr_->message_);
    return true;
}
