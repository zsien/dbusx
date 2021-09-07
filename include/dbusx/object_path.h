#ifndef DBUSX_OBJECT_PATH_H
#define DBUSX_OBJECT_PATH_H

#include <string>

namespace dbusx {

class object_path {
public:
    object_path(const char *s);

    bool is_valid() const;

    const char *c_str() const { return str_.c_str(); }

protected:
    std::string str_;
};

} // namespace dbusx

#endif // !DBUSX_OBJECT_PATH_H
