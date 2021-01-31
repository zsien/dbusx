#ifndef DBUSX_MESSAGE_H
#define DBUSX_MESSAGE_H

#include <string>
#include <memory>

namespace dbusx {

class message_private;

class message {
public:
    explicit message(std::unique_ptr<message_private> &&ptr);

    uint8_t get_byte();
    bool get_bool();
    int16_t get_int16();
    uint16_t get_uint16();
    int32_t get_int32();
    uint32_t get_uint32();
    int64_t get_int64();
    uint64_t get_uint64();
    double get_double();
    std::string get_string();
    std::string get_object_path();
    std::string get_signature();
    int get_fd();

private:
    std::unique_ptr<message_private> d_ptr_;
};

} // namespace dbusx

#endif //! DBUSX_MESSAGE_H
