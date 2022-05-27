#ifndef DBUSX_BUS_H
#define DBUSX_BUS_H

#include <memory>

namespace dbusx {

class bus_private;
struct data;

class interface;
class message;

enum class bus_type { USER, SYSTEM, STARTER };

/*!
  D-Bus connection
 */
class bus {
    friend bus_private;

public:
    bus(bus_type type);
    ~bus();

    /*!
      @brief Request a well-known service name on the bus
      
      @param name the service name
      @return request result
     */
    bool request_name(const std::string &name);

    /*!
      @brief Release a well-known service name on the bus
      
      @param name the service name
      @return release result
     */
    bool release_name(const std::string &name);

    /*!
      @brief Export the interface object on the bus

      @param path the path of the interface
      @param iface the interface name
      @param obj the interface object
     */
    bool export_interface(const std::string &path, const std::string &iface, interface *obj);

    /*!
      @brief Start the event loop
     */
    void start();

private:
    std::unique_ptr<bus_private> d_ptr_;
};

} // namespace dbusx

#endif // !DBUSX_BUS_H
