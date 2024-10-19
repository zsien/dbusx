#ifndef DBUSX_MESSAGE_H
#define DBUSX_MESSAGE_H

#include "utils.h"
#include "typedef.h"
#include "type.h"

#include <memory>
#include <string>

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
        } else if constexpr (std::is_same<Type, fd>::value) {
            return read_fd();
        } else if constexpr (std::is_same<Type, std::string>::value) {
            return read_string();
        } else if constexpr (std::is_same<Type, object_path>::value) {
            return read_object_path();
        } else if constexpr (std::is_same<Type, signature>::value) {
            return read_signature();
        } else if constexpr (is_vector<Type>::value) {
            return read_container<Type>();
        } else if constexpr (is_unordered_map<Type>::value) {
            return read_container<Type>();
        } else if constexpr (is_tuple<Type>::value) {
            return read_container<Type>();
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    }

    template <typename T>
    bool append(T &&v) {
        using Type = std::remove_cvref_t<T>;

        if constexpr (std::is_same<Type, char>::value) {
            return append_byte(v);
        } else if constexpr (std::is_same<Type, bool>::value) {
            return append_bool(v);
        } else if constexpr (std::is_same<Type, int16_t>::value) {
            return append_int16(v);
        } else if constexpr (std::is_same<Type, uint16_t>::value) {
            return append_uint16(v);
        } else if constexpr (std::is_same<Type, int32_t>::value) {
            return append_int32(v);
        } else if constexpr (std::is_same<Type, uint32_t>::value) {
            return append_uint32(v);
        } else if constexpr (std::is_same<Type, int64_t>::value) {
            return append_int64(v);
        } else if constexpr (std::is_same<Type, uint64_t>::value) {
            return append_uint64(v);
        } else if constexpr (std::is_same<Type, double>::value) {
            return append_double(v);
        } else if constexpr (std::is_same<Type, fd>::value) {
            return append_fd(v);
        } else if constexpr (std::is_same<Type, std::string>::value) {
            return append_string(std::forward<T>(v));
        } else if constexpr (std::is_same<Type, object_path>::value) {
            return append_object_path(std::forward<T>(v));
        } else if constexpr (std::is_same<Type, signature>::value) {
            return append_signature(std::forward<T>(v));
        } else if constexpr (is_vector<Type>::value) {
            return append_container(std::forward<T>(v));
        } else if constexpr (is_unordered_map<Type>::value) {
            return append_container(std::forward<T>(v));
        } else if constexpr (is_tuple<Type>::value) {
            return append_container(std::forward<T>(v));
        } else {
            static_assert(always_false_v<Type>, "unsupported type");
        }
    }

    template <typename... T>
    bool append(T &&...a) {
        if constexpr (all_pod<T...>::value) {
            return c_append(types<T...>::signature_nt.data(), a...);
        }

        return (... && append(std::forward<T>(a)));
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
    fd read_fd() const;

    template <typename T>
    T read_container() const {
        using Type = std::remove_cvref_t<T>;

        Type res;

        if constexpr (is_vector<Type>::value) {
            enter_container(dbusx::type<Type>::signature_nt[0],
                            dbusx::type<Type>::signature_nt.data() + 1);
            while (!at_end()) {
                res.emplace_back(read<typename Type::value_type>());
            }
            exit_container();
        } else if constexpr (is_unordered_map<Type>::value) {
            enter_container(dbusx::type<Type>::signature_nt[0],
                            dbusx::type<Type>::signature_nt.data() + 1);
            while (!at_end()) {
                enter_container(dbusx::type<Type, true>::signature_nt[1],
                                dbusx::type<Type, true>::signature_nt.data() + 2);
                res.emplace(
                    std::pair{read<typename Type::key_type>(), read<typename Type::mapped_type>()});
                exit_container();
            }
            exit_container();
        } else if constexpr (is_tuple<Type>::value) {
            enter_container(dbusx::type<Type, true>::signature_nt[0],
                            dbusx::type<Type, true>::signature_nt.data() + 1);
            std::apply([this](auto &...i) { (..., (i = read<decltype(i)>())); }, res);
            exit_container();
        }

        return res;
    }

    bool append_byte(char y);
    bool append_bool(bool b);
    bool append_int16(int16_t n);
    bool append_uint16(uint16_t q);
    bool append_int32(int32_t i);
    bool append_uint32(uint32_t u);
    bool append_int64(int64_t x);
    bool append_uint64(uint64_t t);
    bool append_double(double d);
    bool append_fd(fd f);
    bool append_string(const std::string &s);
    bool append_object_path(const object_path &o);
    bool append_signature(const signature &g);

    template <typename T>
    bool append_container(T &&c) {
        using Type = std::remove_cvref_t<T>;

        if constexpr (is_vector<Type>::value) {
            bool res = open_container(dbusx::type<Type>::signature_nt[0],
                                      dbusx::type<Type>::signature_nt.data() + 1);
            if (!res) {
                return res;
            }

            for (const typename Type::value_type &i : c) {
                res = append(i);
                if (!res) {
                    break;
                }
            }

            return close_container();
        } else if constexpr (is_unordered_map<Type>::value) {
            bool res = open_container(dbusx::type<Type>::signature_nt[0],
                                      dbusx::type<Type>::signature_nt.data() + 1);
            if (!res) {
                return res;
            }

            for (const auto &[k, v] : c) {
                res = open_container(dbusx::type<Type, true>::signature_nt[1],
                                     dbusx::type<Type, true>::signature_nt.data() + 2);
                if (!res) {
                    break;
                }

                res = append(k);
                if (!res) {
                    break;
                }

                res = append(v);
                if (!res) {
                    break;
                }

                res = close_container();
                if (!res) {
                    break;
                }
            }

            return close_container();
        } else if constexpr (is_tuple<Type>::value) {
            bool res = open_container(dbusx::type<Type, true>::signature_nt[0],
                                      dbusx::type<Type, true>::signature_nt.data() + 1);
            if (!res) {
                return res;
            }

            res = std::apply(
                [this](auto &&...i) { return (... && append(std::forward<decltype(i)>(i))); },
                std::forward<T>(c));
            if (!res) {
                return res;
            }

            return close_container();
        }
    }

    bool c_append(const char *signature, ...);

    bool send();

private:
    std::unique_ptr<message_private> d_ptr_;

    void enter_container(char type, const char *contents) const;
    void exit_container() const;
    bool at_end(bool current = true) const;

    bool open_container(char type, const char *contents);
    bool close_container();
};

} // namespace dbusx

#endif //! DBUSX_MESSAGE_H
