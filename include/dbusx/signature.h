#ifndef DBUSX_SIGNATURE_H
#define DBUSX_SIGNATURE_H

#include <string>

#include "utils.h"

namespace dbusx {

class signature {
public:
    signature(const char *s);

protected:
    std::string str_;
};

} // namespace dbusx

#endif // !DBUSX_SIGNATURE_H
