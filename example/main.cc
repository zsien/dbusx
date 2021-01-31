#include <iostream>

#include <dbusx/bus.h>
#include <dbusx/method.h>
#include <dbusx/signature.h>

#include "example.h"

int main(int argc, char *argv[]) {
    dbusx::bus bus(dbusx::type::user);
    bus.request_name("cn.zsien.dbusx.example");
    example a;

    bus.export_interface("/cn/zsien/dbusx/example", "cn.zsien.dbusx.example.control", &a);

    bus.start();

    return 0;
}
