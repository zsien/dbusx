#ifndef EXAMPLE_EXAMPLE_H
#define EXAMPLE_EXAMPLE_H

#include <dbusx/interface.h>

class example : public dbusx::interface {
public:
    example();

    std::tuple<int, uint64_t> one_method(int, int, std::string);

    virtual std::vector<dbusx::vtable::vtable> exported() override;
};

#endif // !EXAMPLE_EXAMPLE_H
