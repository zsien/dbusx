#include <iostream>

#include <dbusx/bus.h>

#include "example.h"

int main() {
    dbusx::bus bus(dbusx::bus_type::USER);
    bus.request_name("cn.zsien.dbusx.example");

    example a;
    bus.export_interface("/cn/zsien/dbusx/example", "cn.zsien.dbusx.example.control", &a);

    bus.start();

    return 0;
}
