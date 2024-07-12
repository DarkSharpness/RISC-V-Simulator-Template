# Some common mistakes

## Bit-width mismatch

That is, the bit-width of the LHS and RHS of an assignment operation are different.
For example, the following code will result in compile error:

```cpp
Register <8>    r1;
Register <16>   r2;
r1 <= r2; // Error: bit-width mismatch
```

## Register/Wire passed by value

Register/Wire can be only passed by reference. We forbid
the copy/move constructor for Register/Wire to avoid misuse.

This may cause some error in the lambda function of a wire.

```cpp
Register <8> r1;
Wire <8> w1 = [&]() { return r1; };
```

To fix this issue, you may return by reference,
or use + operator to convert the value to bit type.

```cpp
Register <8> r1;
Wire <8> w1 = [&]() -> auto & { return r1; };
Wire <8> w2 = [&]() { return +r1; };
```

## C-array as member variable

We do not support C-array as member variable for synchronization.
Our C++ static reflection library do not support parsing
C-array as member variable currently.

Always use `std::array` instead.

```cpp
struct NeedToSync {
    std::array <Register <1>, 8> data;
    std::array <Wire <1>, 8> data2;
};
```

## Some others

If you encounter some other issues, please feel free to open an issue.
