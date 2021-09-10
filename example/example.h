#ifndef EXAMPLE_EXAMPLE_H
#define EXAMPLE_EXAMPLE_H

#include <dbusx/interface.h>

class example : public dbusx::interface {
public:
    example();

    uint32_t string_length(const std::string &str);
    int32_t always_error(int32_t);
    std::string echo(const std::string &str);

    std::string get_read_only_propery();
    std::string get_read_only_propery_error();

    virtual std::string interface_name() override;
    virtual dbusx::vtable::vtable exported() override;
};

#endif // !EXAMPLE_EXAMPLE_H
