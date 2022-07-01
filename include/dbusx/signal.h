#ifndef DBUSX_SIGNAL_H
#define DBUSX_SIGNAL_H

namespace dbusx {

template <typename... T>
class signal {
public:
    void emit(T &...v) {
        
    }
};

} // namespace dbusx

#endif // !DBUSX_SIGNAL_H
