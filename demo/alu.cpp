#include "../include/tools.h"
#include <iostream>

bool reset;
bool ready;

// RISC-V
enum class Opcode : dark::max_size_t {
    ADD,
    SUB,
    SLL,
    SRL,
    SRA,
    AND,
    OR,
    XOR,
    SLT,
    SLTU,
    SGE,
    SGEU,
    SEQ,
    SNEQ
};


// Normally, only wire can be used in the input.
struct AluInput {
    Wire <8> opcode;
    Wire <1> issue;
    Wire <32> rs1;
    Wire <32> rs2;
};

struct AluOutput {
    Register <32> out;
    Register <1> done;    
};

struct AluModule : AluInput, AluOutput {
    using Tags = SyncTags<AluInput, AluOutput>;

    void work() {
        if (reset) {
            done <= 0;
        } else if(ready && issue) {
            switch (static_cast <Opcode> (static_cast <unsigned> (opcode))) {
                using enum Opcode;
                case ADD:   out <= (rs1 + rs2); break;
                case SUB:   out <= (rs1 - rs2); break;
                case SLL:   out <= (rs1 << rs2); break;
                case SRL:   out <= (rs1 >> rs2); break;
                case SRA:   out <= (to_signed(rs1) >> to_unsigned(rs2));
                case AND:   out <= (rs1 & rs2); break;
                case OR:    out <= (rs1 | rs2); break;
                case XOR:   out <= (rs1 ^ rs2); break;
                case SLT:   out <= (to_signed(rs1) < to_signed(rs2)); break;
                case SLTU:  out <= (rs1 < rs2); break;
                case SGE:   out <= (to_signed(rs1) >= to_signed(rs2)); break;
                case SGEU:  out <= (rs1 >= rs2); break;
                case SEQ:   out <= (rs1 == rs2); break;
                case SNEQ:  out <= (rs1 != rs2); break;
                default: dark::debug::assert(false, "Invalid opcode");
            }
            done <= 1;
        } else {
            done <= 0;
        }
    }

};

signed main() {
    AluModule alu;

    std::string opstring;

    max_size_t opcode;
    max_size_t issue;
    max_size_t rs1;
    max_size_t rs2;

    ready = 1;

    alu.opcode = [&]() { return opcode; };
    alu.issue = [&]() { return issue; };
    alu.rs1 = [&]() { return rs1; };
    alu.rs2 = [&]() { return rs2; };

    while (std::cin >> opstring) {
        issue = 1;
        std::cin >> rs1 >> rs2;
        if (opstring == "add") {
            opcode = static_cast <max_size_t> (Opcode::ADD);
        } else if (opstring == "sub") {
            opcode = static_cast <max_size_t> (Opcode::SUB);
        } else if (opstring == "sll") {
            opcode = static_cast <max_size_t> (Opcode::SLL);
        } else if (opstring == "srl") {
            opcode = static_cast <max_size_t> (Opcode::SRL);
        } else if (opstring == "sra") {
            opcode = static_cast <max_size_t> (Opcode::SRA);
        } else if (opstring == "and") {
            opcode = static_cast <max_size_t> (Opcode::AND);
        } else if (opstring == "or") {
            opcode = static_cast <max_size_t> (Opcode::OR);
        } else if (opstring == "xor") {
            opcode = static_cast <max_size_t> (Opcode::XOR);
        } else if (opstring == "slt") {
            opcode = static_cast <max_size_t> (Opcode::SLT);
        } else if (opstring == "sltu") {
            opcode = static_cast <max_size_t> (Opcode::SLTU);
        } else if (opstring == "sge") {
            opcode = static_cast <max_size_t> (Opcode::SGE);
        } else if (opstring == "sgeu") {
            opcode = static_cast <max_size_t> (Opcode::SGEU);
        } else if (opstring == "seq") {
            opcode = static_cast <max_size_t> (Opcode::SEQ);
        } else if (opstring == "sneq") {
            opcode = static_cast <max_size_t> (Opcode::SNEQ);
        } else {
            std::cout << "Invalid opcode" << std::endl;
            issue = 0;
        }

        alu.work();

        std::cout << to_unsigned(alu.out) << std::endl;
        sync_member(alu);
    }

}