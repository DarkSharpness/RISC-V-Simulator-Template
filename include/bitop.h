#pragma once
#include "bit.h"

namespace dark::bits {

template <typename _Tp>
concept NonBit = !BitType<_Tp>;

template <typename _Tp>
auto cast(_Tp &&val) -> target_size_t {
    return static_cast<target_size_t>(val);
}

template <BitType _Lhs, BitType _Rhs>
constexpr auto operator + (_Lhs lhs, _Rhs rhs) {
    static_assert(_Lhs::_Bit_Len == _Rhs::_Bit_Len,
        "operator +: lhs and rhs should have the same length");
    return _Lhs { cast(lhs) + cast(rhs) };
}

template <BitType _Lhs, NonBit _Rhs>
constexpr auto operator + (_Lhs lhs, _Rhs &&rhs) {
    return _Lhs { cast(lhs) + cast(rhs) };
}

template <NonBit _Lhs, BitType _Rhs>
constexpr auto operator + (_Lhs &&lhs, _Rhs rhs) {
    return _Rhs { cast(lhs) + cast(rhs) };
}


} // namespace dark::bits
