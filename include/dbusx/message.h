#ifndef DBUSX_MESSAGE_H
#define DBUSX_MESSAGE_H

#include <memory>
#include <string>

#include "utils.h"
#include "type.h"

namespace dbusx {

class message_private;

class error;

/*!
  @brief DBus message
 */
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
        } else if constexpr (std::is_same<Type, object_path>::value) {
            return read_object_path();
        } else if constexpr (std::is_same<Type, signature>::value) {
            return read_signature();
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    }

    template <typename T>
    bool append(T &a) {
        using Type = std::remove_cvref_t<T>;

        if constexpr (std::is_same<Type, char>::value) {
            return append_byte(a);
        } else if constexpr (std::is_same<Type, bool>::value) {
            return append_bool(a);
        } else if constexpr (std::is_same<Type, int16_t>::value) {
            return append_int16(a);
        } else if constexpr (std::is_same<Type, uint16_t>::value) {
            return append_uint16(a);
        } else if constexpr (std::is_same<Type, int32_t>::value) {
            return append_int32(a);
        } else if constexpr (std::is_same<Type, uint32_t>::value) {
            return append_uint32(a);
        } else if constexpr (std::is_same<Type, int64_t>::value) {
            return append_int64(a);
        } else if constexpr (std::is_same<Type, uint64_t>::value) {
            return append_uint64(a);
        } else if constexpr (std::is_same<Type, double>::value) {
            return append_double(a);
            // todo: UNIX_FD
            // } else if constexpr () {
            //     return std::array{'h'};
        } else if constexpr (std::is_same<Type, std::string>::value) {
            return append_string(std::move(a));
        } else if constexpr (std::is_same<Type, object_path>::value) {
            return append_object_path(std::move(a));
        } else if constexpr (std::is_same<Type, signature>::value) {
            return append_signature(std::move(a));
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    }

    template <typename... T>
    bool append(T &...a) {
        if constexpr (all_pod<T...>::value) {
            return c_append(types<T...>::signature_nt.data(), std::forward(a)...);
        }

        return (... && append(std::forward(a)));
    }

    message();
    explicit message(std::unique_ptr<message_private> &&ptr);
    ~message();

    message(message &&r) noexcept;
    message &operator=(message &&r) noexcept;

    message create_return() const;
    message create_error(const error &error) const;

    std::string get_destination() const;
    std::string get_path() const;
    std::string get_interface() const;
    std::string get_member() const;
    std::string get_sender() const;

    type get_type() const;

    char read_byte() const;
    bool read_bool() const;
    int16_t read_int16() const;
    uint16_t read_uint16() const;
    int32_t read_int32() const;
    uint32_t read_uint32() const;
    int64_t read_int64() const;
    uint64_t read_uint64() const;
    double read_double() const;
    std::string read_string() const;
    object_path read_object_path() const;
    signature read_signature() const;
    int read_fd() const;

    bool append_byte(char y);
    bool append_bool(bool b);
    bool append_int16(int16_t n);
    bool append_uint16(uint16_t q);
    bool append_int32(int32_t i);
    bool append_uint32(uint32_t u);
    bool append_int64(int64_t x);
    bool append_uint64(uint64_t t);
    bool append_double(double d);
    bool append_string(const std::string &s);
    bool append_object_path(const object_path &o);
    bool append_signature(const signature &g);

    bool c_append(const char *signature, ...);

    bool send();

private:
    std::unique_ptr<message_private> d_ptr_;
};

} // namespace dbusx

#endif //! DBUSX_MESSAGE_H
