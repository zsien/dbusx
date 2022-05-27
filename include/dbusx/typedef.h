#ifndef DBUSX_TYPEDEF_H
#define DBUSX_TYPEDEF_H

#include <string>

// from boost
#define STRONG_TYPEDEF(T, D)                                                                       \
    struct D {                                                                                     \
        T t;                                                                                       \
        explicit D(const T t_)                                                                     \
            : t(t_){};                                                                             \
        D(){};                                                                                     \
        D(const D &t_)                                                                             \
            : t(t_.t) {}                                                                           \
        D &operator=(const D &rhs) {                                                               \
            t = rhs.t;                                                                             \
            return *this;                                                                          \
        }                                                                                          \
        D &operator=(const T &rhs) {                                                               \
            t = rhs;                                                                               \
            return *this;                                                                          \
        }                                                                                          \
        operator const T &() const { return t; }                                                   \
        operator T &() { return t; }                                                               \
        bool operator==(const D &rhs) const { return t == rhs.t; }                                 \
        bool operator<(const D &rhs) const { return t < rhs.t; }                                   \
    };

namespace dbusx {

STRONG_TYPEDEF(int, fd);
STRONG_TYPEDEF(std::string, signature);
STRONG_TYPEDEF(std::string, object_path);

} // namespace dbusx

#undef STRONG_TYPEDEF

#endif // !DBUSX_TYPEDEF_H
