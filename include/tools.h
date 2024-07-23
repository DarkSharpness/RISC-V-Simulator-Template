#pragma once
#include "bit.h"
#include "bit_impl.h"
#include "wire.h"
#include "register.h"
#include "synchronize.h"
#include "operator.h"

using dark::Bit;
using dark::sign_extend;
using dark::zero_extend;

using dark::Wire;
using dark::Register;

using dark::SyncTags;
using dark::sync_member;
using dark::Visitor;

using dark::max_size_t;
using dark::max_ssize_t;

template <dark::concepts::bit_type _Tp>
constexpr auto to_unsigned(const _Tp &x) {
    return static_cast<dark::max_size_t>(x);
}

template <dark::concepts::bit_type _Tp>
constexpr auto to_signed(const _Tp &x) {
    return static_cast<dark::max_ssize_t>(to_unsigned(x));
}
