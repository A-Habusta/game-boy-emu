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
    void register_file::write_to_register(whole_register_name target_register, word value) {
        switch (target_register) {
            case whole_register_name::AF: whole.AF = value & AF_mask; break;
            case whole_register_name::BC: whole.BC = value; break;
            case whole_register_name::DE: whole.DE = value; break;
            case whole_register_name::HL: whole.HL = value; break;
            case whole_register_name::SP: whole.SP = value; break;
            case whole_register_name::PC: whole.PC = value; break;
            default: break;
        }
    }

    byte register_file::read_from_register(half_register_name target_register) const {
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

    word register_file::read_from_register(whole_register_name target_register) const {
        switch (target_register) {
            case whole_register_name::AF: return whole.AF & AF_mask;
            case whole_register_name::BC: return whole.BC;
            case whole_register_name::DE: return whole.DE;
            case whole_register_name::HL: return whole.HL;
            case whole_register_name::SP: return whole.SP;
            case whole_register_name::PC: return whole.PC;
            default: return 0;
        }
    }
}