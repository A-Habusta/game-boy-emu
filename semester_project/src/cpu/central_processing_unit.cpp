// File: central_processing_unit.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "../utility.hpp"
#include "central_processing_unit.hpp"


void central_processing_unit::cpu::reset() {
    registers.whole.AF = 0x01B0;
    registers.whole.BC = 0x0013;
    registers.whole.DE = 0x00D8;
    registers.whole.HL = 0x014D;
    registers.whole.SP = 0xFFFE;
    registers.whole.PC = 0x0100;
    next_instruction = read_byte_from_pc();
}


void central_processing_unit::cpu::prefetch_next_instruction() {
    next_instruction = read_byte_from_pc();
}

void central_processing_unit::cpu::execute_instruction() {
    // A switch is used because I expect the compiler to optimize it to a jump table. If the emulator
    // is too slow, I will change it to a function pointer array.
    switch (next_instruction) {
        // Cases are grouped by instruction type not by order

    }

    prefetch_next_instruction();
}

