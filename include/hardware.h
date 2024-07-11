#pragma once
#include "debug.h"
#include "concept.h"
#include <memory>
#include <cstdint>
#include <concepts>

namespace dark::hardware {

struct Visitor;
struct Wire;
struct Register;

template <typename _Fn>
concept WireFunction =
    !std::same_as <Wire, std::decay_t <_Fn>> &&
    requires(_Fn &&__f) { { __f() } -> std::convertible_to <max_size_t>; };

struct WireBase {
    using _Ret_t = int;
    using _Cpy_t = WireBase *;
    virtual _Ret_t call() const = 0;
    virtual ~WireBase() = default;
};

template <WireFunction _Fn>
struct WireImpl final : WireBase {
    _Fn _M_lambda;

    template <typename _Fn2>
    WireImpl(_Fn2 &&fn) : _M_lambda(std::forward <_Fn2>(fn)) {}

    _Ret_t call() const override { return this->_M_lambda(); }
};

struct EmptyWire final : WireBase {
    _Ret_t call() const override {
        debug::assert(false, "Empty wire is called.");
        debug::unreachable();
    }
};

struct Wire {
  private:
    friend struct Visitor;

    using _Manage_t = WireBase;

    std::unique_ptr <_Manage_t>     _M_impl;
    mutable max_size_t           _M_cache;
    mutable bool                    _M_holds;

    [[no_unique_address]]
    debug::DebugValue <bool, false> _M_dirty; // Can be assigned only once

    void sync() { this->_M_holds = false; }

    template <WireFunction _Fn>
    static auto _M_new_impl(_Fn &&fn) -> _Manage_t * {
        return new WireImpl <std::decay_t<_Fn>> {std::forward <_Fn>(fn)};
    }

    void _M_check_double_assign() {
        debug::assert(!this->_M_dirty, "Wire is already assigned.");
        this->_M_dirty = true;
    }

  public:
    Wire() : _M_impl(new EmptyWire), _M_cache(), _M_holds(), _M_dirty() {}

    Wire(Wire &&) = delete;
    Wire(const Wire &) = delete;
    Wire &operator=(Wire &&) = delete;
    Wire &operator=(const Wire &rhs) = delete;

    template <WireFunction _Fn>
    Wire(_Fn &&fn)
        : _M_impl(_M_new_impl(std::forward <_Fn>(fn))), _M_cache(), _M_holds(), _M_dirty() {}

    template <WireFunction _Fn>
    Wire &operator=(_Fn &&fn) {
        this->assign(std::forward <_Fn>(fn));
        return *this;
    }

    template <WireFunction _Fn>
    void assign(_Fn &&fn) {
        this->_M_check_double_assign();
        this->_M_impl.reset(this->_M_new_impl(std::forward <_Fn>(fn)));
        this->_M_holds = false;
    }

    operator max_size_t() const {
        if (this->_M_holds == false) {
            this->_M_cache = this->_M_impl->call();
            this->_M_holds = true;
        }
        return this->_M_cache;
    }
};

struct Register {
  private:
    friend struct Visitor;

    max_size_t _M_new;
    max_size_t _M_old;

    [[no_unique_address]]
    debug::DebugValue <bool, false> _M_dirty;

    void sync() {
        if (this->_M_dirty) {
            this->_M_old = this->_M_new;
            this->_M_dirty = false;
        }
    }

    void set_value(max_size_t value) {
        this->_M_new = value;
        debug::assert(!this->_M_dirty, "Register is already assigned in this cycle.");
        this->_M_dirty = true;
    }

    auto get_value() const -> max_size_t { return this->_M_old; }

  public:

    Register() : _M_new(), _M_old(), _M_dirty() {}

    Register(Register &&) = delete;
    Register(const Register &) = delete;
    Register &operator=(Register &&) = delete;
    Register &operator=(const Register &rhs) = delete;

    template <std::convertible_to <max_size_t> _Int>
    void operator <= (_Int &&value) {
        this->set_value(static_cast <max_size_t>(value));
    }

    operator max_size_t() const { return this->get_value(); }
};

} // namespace dark::hardware
