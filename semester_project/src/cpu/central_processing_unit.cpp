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

    void cpu::execute() {
        if (queued_ime_enable) {
            enable_interrupts();
        }

        interrupt_type interrupt = check_for_interrupts();
        handle_interrupts(interrupt);

        execute_instruction();

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

    cpu::interrupt_type cpu::check_for_interrupts() {
        if (!interrupt_master_enable)
            return NONE;

        byte interrupt_requested_register = read_interrupt_requested_register_raw();
        byte interrupt_enabled_register = read_interrupt_enable_register_raw();

        // Interrupts are serviced with priority going from interrupt in bit 0 -> bit 4
        for (int i = 0; i < interrupt_types_count; ++i) {
            bool interrupt_requested = utility::get_bit(interrupt_requested_register, i);
            bool interrupt_enabled = utility::get_bit(interrupt_enabled_register, i);

            if (interrupt_requested && interrupt_enabled)
                return static_cast<interrupt_type>(i);
        }

        return NONE;
    }

    void cpu::handle_interrupts(interrupt_type interrupt) {
        if (interrupt == NONE)
            return;

        disable_interrupts();

        word interrupt_handler_address = interrupt_jump_targets[interrupt];

        // 1 cycle
        run_phantom_cycle();

        // 3 cycles
        call(interrupt_handler_address, true);

        // 1 cycle
        prefetch_next_instruction();
    }

}