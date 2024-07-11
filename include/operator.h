#pragma once
#include "bit.h"

namespace dark {

using dark::concepts::bit_type;
using dark::concepts::int_type;
using dark::concepts::bit_match;

template <typename _Tp>
constexpr auto cast(const _Tp& value) {
    return static_cast <max_size_t> (value);
}

template <typename _Tp, typename _Up>
consteval auto get_common_length() -> std::size_t {
    static_assert(bit_match <_Tp, _Up>);
    if constexpr (bit_type <_Tp>) {
        return _Tp::_Bit_Len;
    } else {
        static_assert(bit_type <_Up>, "Invalid common length");
        return _Up::_Bit_Len;
    }
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator + (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator - (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator * (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator / (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator & (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator | (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

template <typename _Tp, typename _Up>
requires bit_match <_Tp, _Up>
constexpr auto operator ^ (const _Tp &lhs, const _Up &rhs) {
    constexpr auto _Len = get_common_length <_Tp, _Up>();
    return Bit <_Len> (cast(lhs) + cast(rhs));
}

} // namespace dark
