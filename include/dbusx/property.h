#ifndef DBUSX_PROPERTY_H
#define DBUSX_PROPERTY_H

#include "vtable.h"
#include "interface.h"
#include "message.h"
#include "error.h"

#include <tl/expected.hpp>

#include <optional>

namespace dbusx {

template <auto...>
struct property;

/*!
  @brief a wrapper to generate a vtable item of readonly property

  @tparam GETTER property getter method, signature:
          \code PROP_TYPE (CLASS::*)() \endcode or
          \code tl::expected<PROP_TYPE, dbusx::error> (CLASS::*)() \endcode
*/
template <typename C, typename RET, RET (C::*GETTER)()>
struct property<GETTER> {
    static vtable::property get_vtable() {
        return {
            .signature = type<typename return_type<RET>::type>::signature_str.c_str(),
            .getter = get,
            .setter = nullptr,
            .flags = 0,
        };
    }

    static std::optional<error> get(interface *o, message &m) {
        C *obj = reinterpret_cast<C *>(o);
        RET r = (obj->*GETTER)();

        if constexpr (tl::detail::is_expected<RET>::value) {
            if (r) {
                m.append(r.value());
            } else {
                return r.error();
            }
        } else {
            m.append(r);
        }

        return std::nullopt;
    }
};

/*!
  @brief A wrapper to generate a vtable item of writable property

  @tparam GETTER property getter method, signature:
          \code PROP_TYPE (CLASS::*)() \endcode or
          \code tl::expected<PROP_TYPE, dbusx::error> (CLASS::*)() \endcode
  @tparam SETTER property setter method, signature:
          \code void (CLASS::*)(PROP_TYPE value) \endcode or
          \code std::optional<dbusx::error> (CLASS::*)(PROP_TYPE value) \endcode
 */
template <typename C,
          typename RET,
          typename SETTER_RET,
          RET (C::*GETTER)(),
          SETTER_RET (C::*SETTER)(const RET &)>
struct property<GETTER, SETTER> : public property<GETTER> {
    static vtable::property get_vtable() {
        return {
            .signature = type<typename return_type<RET>::type>::signature_str.c_str(),
            .getter = property<GETTER>::get,
            .setter = set,
            .flags = 0,
        };
    }

    static std::optional<error> set(interface *o, message &m) {
        C *obj = reinterpret_cast<C *>(o);

        // constexpr if won't do any short circuit logic
        if constexpr (is_optional<SETTER_RET>::value) {
            if constexpr (std::is_same<typename SETTER_RET::value_type, error>::value) {
                return (obj->*SETTER)(m.read<RET>());
            } else {
                static_assert(always_false_v<RET>, "unsupported type");
            }
        } else if constexpr (std::is_same<SETTER_RET, void>::value) {
            (obj->*SETTER)(m.read<RET>());

            return std::nullopt;
        } else {
            static_assert(always_false_v<RET>, "unsupported type");
        }

        return std::nullopt;
    }
};

} // namespace dbusx

#endif // !DBUSX_PROPERTY_H
