#ifndef DBUSX_OBJECT_PATH_H
#define DBUSX_OBJECT_PATH_H

#include <string>

namespace dbusx {

class object_path {
public:
    object_path(const char *s);

    bool is_valid() const;

protected:
    std::string str_;
};

} // namespace dbusx

#endif // !DBUSX_OBJECT_PATH_H
