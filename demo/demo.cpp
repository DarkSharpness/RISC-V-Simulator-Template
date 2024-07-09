#include "../include/tools"

struct Input {
    Wire a;
    Wire b;
    std::array <Wire, 2> c;
};

struct Output {
    Register d;
    Register e;
};

struct Content {
    Register r;
};

struct MyModule : Input, Output, private Content {
    using Tags = SyncTags <Input, Output, Content>;
    friend class Visitor;

    void demo() {
        this->d <= this->a + this->b;
    }
};

signed main() {
    MyModule m;

    m.a = []() { return 1; };
    m.b.assign([&]() { return (target_size_t)m.d; });

    for (int i = 0 ; i < 10 ; ++i) {
        std::cout << m.d << std::endl;
        m.demo();
        std::cout << m.d << std::endl;
        sync_member(m);
    }
    return 0;
}
