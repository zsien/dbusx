#ifndef EXAMPLE_EXAMPLE_H
#define EXAMPLE_EXAMPLE_H

#include <dbusx/interface.h>
#include <dbusx/error.h>
#include <dbusx/typedef.h>

#include <tl/expected.hpp>

class example : public dbusx::interface {
public:
    example();

    uint32_t string_length(const std::string &str);
    std::string echo(const std::string &str);

    tl::expected<std::string, dbusx::error> may_error(bool error);

    dbusx::object_path get_path();
    void no_return();
    tl::expected<void, dbusx::error> no_return_or_error(bool error);

    std::string multi_params(const std::string &str1, const std::string &str2);

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
