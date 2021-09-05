#include "dbusx/object_path.h"

#include <stack>

using namespace dbusx;

object_path::object_path(const char *s)
    : str_(s) {
}

bool object_path::is_valid() const {
    if (str_.empty()) {
        return false;
    }

    if (str_[0] != '/') {
        return false;
    }

    if (str_.length() == 1) {
        return true;
    }

    bool last_is_slash = true;
    for (auto it = str_.cbegin() + 1; it != str_.cend(); it++) {
        if (*it == '/') {
            if (last_is_slash) {
                return false;
            }

            last_is_slash = true;
            continue;
        }

        last_is_slash = false;

        if (*it >= 'a' && *it <= 'z') {
            continue;
        }

        if (*it >= 'A' && *it <= 'Z') {
            continue;
        }

        if (*it >= '0' && *it <= '9') {
            continue;
        }

        if (*it == '_') {
            continue;
        }

        return false;
    }

    if (last_is_slash) {
        return false;
    }

    return true;
}
