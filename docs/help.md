# How to use

You need to keep in mind that all the value types just
behave like normal integers, except that we have a similar
bit-width matching check as the verilog integers.
(e.g. 4-bit register can only be assigned from a 4-bit value)

Also, you should use the recommended way to perform the auto-synchronization,
which can (hope so) save you from writing a lot of duplicated code.

## Requirements

`g++-12` or later. `-std=c++20` or `-std=c++2b`.

e.g. `g++ -std=c++2b ...`

## Include the library

This is a header-only library, which means you just need to include all your required headers in your project.

We strongly recommend you to include `include/tools.h` to simply include all the headers.

```cpp
#include "include/tools.h"
```

## Debug mode

We provide a debug mode, which will perform more checks in the code. To enable that,
just define the macro `_DEBUG` before including the headers.
You may also pass `-D _DEBUG` to the compiling command to define the macro.

```cpp
#define _DEBUG
```

## Value types

You may at first treat all these types as the verilog integers.
You may assume all the types below support basic arithmetic operations,
and will **clip** the value just as the verilog integers operations.

### Register

Registers are just like the registers in the verilog.

To simulate the registers, a `Register` is only allowed to be assigned once in a cycle.

```cpp
// Declare a 32-bit register
// The maximum bit-width depends on the max_size_t
// Currently, the max_size_t is std::uint32_t
Register<32> reg;
reg <= reg + 1; // OK, allow to assign from some value with the same bit-width
Register<16> reg2;
reg <= reg2 * reg2; // Compile error, the bit-width is different (32 vs 16)
```

### Wire

Wires are also similar to the wires in the verilog.

It should be assigned exactly once before reading.
It can accept a function-like input (function pointers/lambdas) to extract the value.

```cpp
// Declare a 4-bit wire
Wire<4> wire;
Register<4> reg;

// OK, assign the value from an integer
// Be careful, the value may be clipped
wire = []() { return 0b11010; }; // Clipped to 0b1010

// OK, assign the value from a register
// When the register's value changes,
// the wire's value will also change
Wire <4> wire2 = [&reg]() -> auto & { return reg; };

// Ill formed! The wire is assigned twice
wire = []() { return 0b11010; };

// Ill formed! Wire can not accept a value
// with different bit-width
Wire <5> wire3 = [&]() -> auto & { return reg + 4; };
```

### Bit

Bit is an intermediate type, which can be used to represent an integer with some bit_width.

```cpp
Bit <5> b = 0b111111; // Clipped to 0b11111
b.set <4, 2> (0b110); // Set bit 4, 3, 2 to 1, 1, 0
b.set <4> (0); // Set bit 4 to 0

Bit <3> c = b.range <3, 1>;     // Copy bit 3, 2, 1 to c
Bit <4> d = b.slice <4> (1);    // Copy 4 bits from bit 1 (bit 4, 3, 2, 1) to d

Bit <1> e = d[0]; // Get the 0-th bit of d

Bit f = { b + 3, c, d }; // Concatenate  b + 3, c, d  from  high to low

```

## Synchronization

We support a feature of auto synchronization, which means that you can
easily synchronize all the members of a class by simply calling the `sync_member` function.

We support 4 types of synchronization:

1. Register / Wire type synchronization.
2. An array (only std::array is supported) of synchronizable objects.
3. A class which contains only synchronizable objects, and satisfies std::is_aggregate.
4. A class which has some basis which are synchronizable objects, and has a special tag.

We will show some examples of 3 and 4.

### Example 1

```cpp
// An aggregate class, just a pure struct with some member functions.
// No constructor! (That means, do not declare any constructor,
// and the compiler will generate a default constructor for you)
// See https://en.cppreference.com/w/cpp/language/aggregate_initialization
// We support at most 14 members currently.
struct case3 {
    Register <3> rs1;
    Register <3> rs2;
    Register <3> rd;
    Wire <3> rs1_data;
    Wire <3> rs2_data;
    Wire <3> rd_data;
    std::array <Register <32>, 32> reg;
};
```

### Example 2

```cpp
struct some_private {
    std::array <Register <16>, 3> private_reg;
};

struct case4 : private some_private, public case3 {
    friend class Visitor;
    using Tags = SyncTags <case3, some_private>;
};

// The synchronization function
void demo() {
    case4 c;
    sync_member(c);
}
```

## Common mistakes

Turn to the [mistake](mistake.md) page to see some common mistakes.

## Examples

See demo folder for more examples.
