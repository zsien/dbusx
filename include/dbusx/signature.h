#ifndef DBUSX_SIGNATURE_H
#define DBUSX_SIGNATURE_H

#include <string>

#include "utils.h"

namespace dbusx {

class signature {
public:
    signature(const char *s);

    const char *c_str() const { return str_.c_str(); }

protected:
    std::string str_;
};

} // namespace dbusx

#endif // !DBUSX_SIGNATURE_H
