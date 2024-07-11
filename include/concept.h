#pragma once
#include <cstdint>
#include <concepts>

namespace dark {

using max_size_t = std::uint32_t;
using max_ssize_t = std::int32_t;

} // namespace dark

namespace dark::concepts {

template <typename _Tp>
using func_t = void(*)(_Tp);

template <typename _From, typename _To>
concept implicit_convertible_to = requires(_From &a, func_t <_To> b) {
    b(a); // Can implicitly convert
};

template <typename _From, typename _To>
concept explicit_convertible_to =
    !implicit_convertible_to <_From, _To>
&&   std::constructible_from <_To, _From>;

template <typename _Tp>
concept has_length = requires { _Tp::_Bit_Len; };

template <typename _Tp>
concept bit_type =  has_length <_Tp> && explicit_convertible_to <_Tp, max_size_t>;

template <typename _Tp>
concept int_type = !has_length <_Tp> && implicit_convertible_to <_Tp, max_size_t>;

template <typename _Lhs, typename _Rhs>
concept bit_match =
    (bit_type <_Lhs> && bit_type <_Rhs> && _Lhs::_Bit_Len == _Rhs::_Bit_Len)
||  (int_type <_Lhs> || int_type <_Rhs>);

} // namespace dark::concepts
