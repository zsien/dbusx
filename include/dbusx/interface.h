#ifndef DBUSX_INTERFACE_H
#define DBUSX_INTERFACE_H

#include <string>

namespace dbusx {

namespace vtable {
struct vtable;
}

/*!
  @brief Base for all D-Bus interface
 */
class interface {
public:
    virtual ~interface() = default;
    virtual std::string interface_name() = 0;
    virtual vtable::vtable exported() = 0;
};

} // namespace dbusx

#endif //! DBUSX_INTERFACE_H
