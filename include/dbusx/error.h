#ifndef DBUSX_ERROR_H
#define DBUSX_ERROR_H

#include <string>

namespace dbusx {

class error {
    friend class message;
    friend class bus_private;

public:
    error(const std::string &name, const std::string &message)
        : name_(name)
        , message_(message) {}

private:
    const std::string name_;
    const std::string message_;
};

} // namespace dbusx

#endif //! DBUSX_ERROR_H
