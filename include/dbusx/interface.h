#ifndef DBUSX_INTERFACE_H
#define DBUSX_INTERFACE_H

#include <string>

namespace dbusx {

namespace vtable {
    struct vtable;
}

class interface {
public:
    virtual std::string interface_name() = 0;
    virtual vtable::vtable exported() = 0;
};

} // namespace dbusx

#endif //! DBUSX_INTERFACE_H
