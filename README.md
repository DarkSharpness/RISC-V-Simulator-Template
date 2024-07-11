# RISC-V Simulator Template

A template which enables you to write verilog-like C++ code.

## How to use

### Include the library

This is a header-only library, which means you just need to include all your required headers in your project.

We strongly recommend you to include `include/tools` to simply include all the headers.

```cpp
#include "include/tools.h"
```

### Debug mode

We provide a debug mode, which will perform more checks in the code. To enable that,
just define the macro `_DEBUG` before including the headers.
You may also pass `-D _DEBUG` to the compiling command to define the macro.

```cpp
#define _DEBUG
```

### Components

You may at first treat all these components as the verilog integers.
You may assume all the types below support basic arithmetic operations,
and will clip the value just as the verilog integers operations.

#### Register

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

#### Wire

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

#### Bit

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

## Deficiencies

- We do not support Combination Circuit directly now. You may simulate that by simpling using normal integers as intermediate values, and arrange a good order to update the values.

- We have not implement all the operators on those `integer-like` types. This will be done in the near future.

- We do no support `signed` types now.

## TODO
