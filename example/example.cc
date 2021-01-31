#include "example.h"

#include <dbusx/method.h>

example::example() {
}

std::vector<dbusx::vtable::vtable> example::exported() {
    return {
        dbusx::method<&example::one_method>::vtable("oneMethod"),
    };
}
