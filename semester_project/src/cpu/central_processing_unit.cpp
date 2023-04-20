// File: central_processing_unit.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../utility.hpp"


namespace central_processing_unit {
    void cpu::reset() {
        registers.whole.AF = 0x01B0;
        registers.whole.BC = 0x0013;
        registers.whole.DE = 0x00D8;
        registers.whole.HL = 0x014D;
        registers.whole.SP = 0xFFFE;
        registers.whole.PC = 0x0100;

        // TODO: This needs to be changed because prefetch emulates a machine cycle and we don't want that
        prefetch_next_instruction();
    }

    void cpu::prefetch_next_instruction() {
        cached_instruction = read_byte_at_pc_with_increment();
    }

    void cpu::enable_interrupts() {
        interrupt_master_enable = true;
    }

    void cpu::disable_interrupts() {
        interrupt_master_enable = false;
        queued_ime_enable = false;
    }
}