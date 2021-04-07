#ifndef DBUSX_ERROR_H
#define DBUSX_ERROR_H

#include <exception>
#include <string>

namespace dbusx {

class error : public std::exception {
    friend class message;

public:
    error(const std::string &name, const std::string &message)
        : name_(name)
        , message_(message) {}

    const char *what() const noexcept override { return message_.c_str(); }

private:
    const std::string name_;
    const std::string message_;
};

} // namespace dbusx

#endif //! DBUSX_ERROR_H
