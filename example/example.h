#ifndef EXAMPLE_EXAMPLE_H
#define EXAMPLE_EXAMPLE_H

#include <dbusx/interface.h>
#include <dbusx/error.h>

#include <tl/expected.hpp>

class example : public dbusx::interface {
public:
    example();

    uint32_t string_length(const std::string &str);
    int32_t always_error(int32_t);
    std::string echo(const std::string &str);

    tl::expected<uint32_t, dbusx::error> string_length_e(const std::string &str);
    tl::expected<int32_t, dbusx::error> always_error_e(int32_t);

    std::string get_read_only_propery();
    tl::expected<std::string, dbusx::error> get_read_only_propery_e();

    std::string get_writable_propery();
    void set_writable_propery(const std::string &str);

    virtual std::string interface_name() override;
    virtual dbusx::vtable::vtable exported() override;

private:
    std::string writable_property_;
};

#endif // !EXAMPLE_EXAMPLE_H
