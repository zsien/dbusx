#ifndef DBUSX_BUS_H
#define DBUSX_BUS_H

#include <memory>

namespace dbusx {

class bus_private;
struct data;

class interface;
class message;

enum class bus_type { USER, SYSTEM, STARTER };

class bus {
    friend bus_private;

public:
    bus(bus_type type);
    ~bus();

    bool request_name(const std::string &name);
    bool release_name(const std::string &name);
    bool export_interface(const std::string &path, const std::string &iface, interface *obj);

    void start();

private:
    std::unique_ptr<bus_private> d_ptr_;
};

} // namespace dbusx

#endif // !DBUSX_BUS_H
