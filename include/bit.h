#pragma once
#include "target.h"
#include "debug.h"
#include <version>
#include <concepts>

namespace dark::bits {

static constexpr std::size_t kMaxLength = 8 * sizeof(target_size_t);

template <int _First>
constexpr auto int_concat(target_size_t arg) { return arg; }

template <int _First, int ..._Lens>
constexpr auto int_concat(target_size_t arg, auto ...args) {
    return (arg << (_Lens + ...)) | int_concat<_Lens...>(args...);
}

template <std::size_t _Nm>
struct Bit {
  private:
    static_assert(0 < _Nm && _Nm <= kMaxLength,
        "Bit: _Nm out of range. Should be in [1, kMaxLength]");
 
    target_size_t _M_data : _Nm; // Real storage

  public:
    static constexpr std::size_t _Bit_Len = _Nm;

    constexpr Bit(target_size_t data = {}) : _M_data(data) {}

    template <std::size_t ..._Lens>
    constexpr Bit(Bit<_Lens> ...args) requires ((_Lens + ...) == _Nm) :
        _M_data(int_concat<_Lens...>(args...)) {}

    constexpr Bit(const Bit &val) = default;

    template <std::size_t _Len>
    constexpr Bit(const Bit <_Len> &val) requires (_Len != _Nm) {
        static_assert(_Len == _Nm, "Bit: bit length mismatch");
    }

    constexpr Bit &operator=(const Bit &val) = default;

    template <std::size_t _Len> requires (_Len != _Nm)
    constexpr Bit &operator=(const Bit <_Len> &val) {
        static_assert(_Len == _Nm, "Bit: bit length mismatch");
        return *this;
    }

    constexpr Bit &operator=(target_size_t val) {
        this->_M_data = val;
        return *this;
    }

    constexpr operator target_size_t() const { return this->_M_data; }

    template <std::size_t _Hi, std::size_t _Lo = _Hi>
    constexpr auto set(Bit <_Hi - _Lo + 1> val) {
        static_cast <void> (this->slice<_Hi, _Lo>());
        const auto mask = // Mask those bit in the middle
            (target_size_t(1) << (_Hi + 1)) - (target_size_t(1) << _Lo);
        const auto data = // Set those bit in the middle
            static_cast <target_size_t> (val) << _Lo;
        this->_M_data = (this->_M_data & ~mask) | data;
    }

    template <std::size_t _Hi, std::size_t _Lo = _Hi>
    constexpr auto slice() const -> Bit <_Hi - _Lo + 1> {
        static_assert(_Lo <= _Hi, "Bit::slice: _Lo should be no greater than _Hi");
        static_assert(_Hi < _Nm, "Bit::slice: _Hi should be less than _Nm");
        return Bit<_Hi - _Lo + 1>(this->_M_data >> _Lo);
    }

    template <std::size_t _Len = 1>
    constexpr auto at(target_size_t pos) const -> Bit <_Len> {
        static_assert(_Len != 0, "Bit::at: _Len should be greater than 0");
        debug::assert(pos + _Len <= _Nm, "Bit::at: pos out of range");
        return Bit <_Len> (this->_M_data >> pos);
    }

    constexpr auto operator [](target_size_t pos) const -> Bit <1> { return this->at(pos); }

    template <std::size_t _New = kMaxLength>
    constexpr auto zero_extend() const -> Bit<_New>;

    template <std::size_t _New = kMaxLength>
    constexpr auto sign_extend() const -> Bit<_New>;
};

template <typename _Tp>
static constexpr bool is_bit_v = false;
template <std::size_t _Nm>
static constexpr bool is_bit_v<Bit<_Nm>> = true;

template <typename _Tp>
concept BitType = is_bit_v<_Tp>;

template <BitType ..._Tp>
Bit(_Tp...) -> Bit<(_Tp::_Bit_Len + ...)>;

template <std::size_t _Old, std::size_t _New = kMaxLength>
constexpr auto sign_extend(target_size_t val) {
    static_assert(_Old < _New, "sign_extend: _Old should be less than _New");
    struct { target_ssize_t _M_data : _Old; } tmp;
    return Bit<_New>(tmp._M_data = val);
}

template <std::size_t _New = kMaxLength, BitType _Tp>
constexpr auto sign_extend(_Tp val) {
    return sign_extend<_Tp::_Bit_Len, _New>(static_cast<target_size_t>(val));
}

template <std::size_t _Old, std::size_t _New = kMaxLength>
constexpr auto zero_extend(target_size_t val) {
    static_assert(_Old < _New, "zero_extend: _Old should be less than _New");
    struct { target_size_t _M_data : _Old; } tmp;
    return Bit<_New>(tmp._M_data = val);
}

template <std::size_t _New = kMaxLength, BitType _Tp>
constexpr auto zero_extend(_Tp val) {
    return zero_extend<_Tp::_Bit_Len, _New>(static_cast<target_size_t>(val));
}

template <std::size_t _Nm>
template <std::size_t _New>
constexpr auto Bit<_Nm>::sign_extend() const -> Bit<_New> {
    return ::dark::bits::sign_extend<_New> (*this);
}

template <std::size_t _Nm>
template <std::size_t _New>
constexpr auto Bit<_Nm>::zero_extend() const -> Bit<_New> {
    return ::dark::bits::zero_extend<_New> (*this);
}

} // namespace dark::bits
