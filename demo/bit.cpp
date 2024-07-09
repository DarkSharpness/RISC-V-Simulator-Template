#include "../include/tools"

signed main() {
    [[maybe_unused]]
    Bit <1> a; // Create a 1-bit object, default to 0

    Bit <10> b(10); // Create a 10-bit object

    auto ref = b.slice <4, 2> (); // A copy of [2, 3, 4] bit

    std::cout << b << std::endl; // 10

    b.set <3, 1> (ref); // Set [1, 2, 3] bit to [2, 3, 4] bit

    std::cout << b << std::endl; // 4

    b.set <0> (~a); // Set the 0-th bit to ~a (= 1 in this case)

    std::cout << b << std::endl; // 5

    auto sec = b.slice <2> (); // A copy of the 2-th bit (= 1 in this case)

    auto d = sec.zero_extend(); // Zero extend (default to 32-bit)

    std::cout << d << std::endl; // 1

    auto c = sec.sign_extend <3> (); // Sign extend (default to 32-bit)

    std::cout << c << std::endl; // 7

    // c + d;   // Error: different size
    // a += 1;  // Error: no assignment-operation operator

    auto e = b - 1; // normal integer can be assumed as any size

    std::cout << e << std::endl; // 4

    return 0;
}
