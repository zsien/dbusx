#ifndef DBUSX_MESSAGE_H
#define DBUSX_MESSAGE_H

#include <memory>
#include <string>

#include "dbusx/utils.h"
#include "dbusx/signature.h"

namespace dbusx {

class message_private;

class message {
public:
    enum class type {
        METHOD_CALL,
        METHOD_RETURN,
        METHOD_ERROR,
        SIGNAL,
    };

    template <typename T>
    std::remove_cvref_t<T> read() const {
        using Type = std::remove_cvref_t<T>;

        if constexpr (std::is_same<Type, char>::value) {
            return read_byte();
        } else if constexpr (std::is_same<Type, bool>::value) {
            return read_bool();
        } else if constexpr (std::is_same<Type, int16_t>::value) {
            return read_int16();
        } else if constexpr (std::is_same<Type, uint16_t>::value) {
            return read_uint16();
        } else if constexpr (std::is_same<Type, int32_t>::value) {
            return read_int32();
        } else if constexpr (std::is_same<Type, uint32_t>::value) {
            return read_uint32();
        } else if constexpr (std::is_same<Type, int64_t>::value) {
            return read_int64();
        } else if constexpr (std::is_same<Type, uint64_t>::value) {
            return read_uint64();
        } else if constexpr (std::is_same<Type, double>::value) {
            return read_double();
            // todo: UNIX_FD
            // } else if constexpr () {
            //     return std::array{'h'};
        } else if constexpr (std::is_same<Type, std::string>::value) {
            return read_string();
            //} else if constexpr (std::is_same<Type, dbus_object_path>::value) {
            //    return std::array{'o'};
            //} else if constexpr (std::is_same<Type, dbus_signature>::value) {
            //    return std::array{'g'};
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    }

    message();
    explicit message(std::unique_ptr<message_private> &&ptr);
    ~message();

    message(message &&r) noexcept;
    message &operator=(message &&r) noexcept;

    message create_return() const;

    std::string get_destination() const;
    std::string get_path() const;
    std::string get_interface() const;
    std::string get_member() const;
    std::string get_sender() const;

    type get_type() const;

    uint8_t read_byte() const;
    bool read_bool() const;
    int16_t read_int16() const;
    uint16_t read_uint16() const;
    int32_t read_int32() const;
    uint32_t read_uint32() const;
    int64_t read_int64() const;
    uint64_t read_uint64() const;
    double read_double() const;
    std::string read_string() const;
    std::string read_object_path() const;
    std::string read_signature() const;
    int read_fd() const;

    template <typename... T>
    bool append(T &... a) {
        if constexpr (all_pod<T...>::value) {
            return c_append(signature_ntstr<T...>.data(), a...);
        }
    }

    bool c_append(const char *signature, ...);

    bool send();

private:
    std::unique_ptr<message_private> d_ptr_;
};

} // namespace dbusx

#endif //! DBUSX_MESSAGE_H
