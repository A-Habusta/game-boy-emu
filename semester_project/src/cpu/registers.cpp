// File: registers.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "registers.hpp"

namespace registers {
    void register_file::write_to_register(half_register_name target_register, byte value) {
        switch (target_register) {
            case half_register_name::A: half.A = value; break;
            case half_register_name::F: half.F = value; break;
            case half_register_name::B: half.B = value; break;
            case half_register_name::C: half.C = value; break;
            case half_register_name::D: half.D = value; break;
            case half_register_name::E: half.E = value; break;
            case half_register_name::H: half.H = value; break;
            case half_register_name::L: half.L = value; break;
            default: break;
        }
    }
    void register_file::write_to_register(word_register target_register, word value) {
        switch (target_register) {
            case word_register::AF: whole.AF = value; break;
            case word_register::BC: whole.BC = value; break;
            case word_register::DE: whole.DE = value; break;
            case word_register::HL: whole.HL = value; break;
            case word_register::SP: whole.SP = value; break;
            case word_register::PC: whole.PC = value; break;
            default: break;
        }
    }

    byte register_file::read_from_register(half_register_name target_register) {
        switch (target_register) {
            case half_register_name::A: return half.A;
            case half_register_name::F: return half.F;
            case half_register_name::B: return half.B;
            case half_register_name::C: return half.C;
            case half_register_name::D: return half.D;
            case half_register_name::E: return half.E;
            case half_register_name::H: return half.H;
            case half_register_name::L: return half.L;
            default: return 0;
        }
    }

    word register_file::read_from_register(word_register target_register) {
        switch (target_register) {
            case word_register::AF: return whole.AF;
            case word_register::BC: return whole.BC;
            case word_register::DE: return whole.DE;
            case word_register::HL: return whole.HL;
            case word_register::SP: return whole.SP;
            case word_register::PC: return whole.PC;
            default: return 0;
        }
    }
}