// File: central_processing_unit.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../utility.hpp"

namespace central_processing_unit {
    cpu::cpu(memory_read_callback read_memory, memory_write_callback write_memory, cycle_callback run_phantom_cycle)
        : read_memory(read_memory), write_memory(write_memory), run_phantom_cycle(run_phantom_cycle) {
    };


    void cpu::reset() {
        registers.whole.AF = 0x01B0;
        registers.whole.BC = 0x0013;
        registers.whole.DE = 0x00D8;
        registers.whole.HL = 0x014D;
        registers.whole.SP = 0xFFFE;
        registers.whole.PC = 0x0100;

        interrupt_master_enable = false;
        interrupt_enable_register = 0;
        interrupt_requested_register = 0;

        // Reset should actually perform one M cycle
        prefetch_next_instruction();
    }

    void cpu::execute() {
        switch (current_state) {
            case state::running: execute_running_state(); break;
            case state::halt_preparation: execute_halt_preparation(); break;
            case state::halted: execute_halted_state(); break;
            case state::crashed: execute_crashed_state(); break;

            // Can't occur
            default: break;
        }
    }

    void cpu::execute_running_state() {
        if (queued_ime_enable) {
            enable_interrupts();
        }

        if (interrupt_master_enable) {
            interrupt_type interrupt = check_for_interrupts();
            handle_interrupts(interrupt);
        }

        execute_instruction(cached_instruction);

        prefetch_next_instruction();
    }

    void cpu::execute_halt_preparation() {
        interrupt_type interrupt = check_for_interrupts();
        if (interrupt == none) {
            run_phantom_cycle();
            current_state = state::halted;
        }
        else {
            current_state = state::running;
        }
    };

    void cpu::execute_halted_state() {
        if (check_for_interrupts() != none) {
            current_state = state::running;
            return;
        }

        run_phantom_cycle();
    }

    void cpu::execute_crashed_state() {
        // CPU is dead, other components continue to function
        run_phantom_cycle();
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
        // Interrupts are serviced with priority going from interrupt in bit 0 -> bit 4
        for (int i = 0; i < interrupt_types_count; ++i) {
            bool interrupt_requested = utility::get_bit(interrupt_requested_register, i);
            bool interrupt_enabled = utility::get_bit(interrupt_enable_register, i);

            if (interrupt_requested && interrupt_enabled) {
                interrupt_type interrupt = static_cast<interrupt_type>(i);

                acknowledge_interrupt(interrupt);
                return interrupt;
            }
        }

        return none;
    }

    void cpu::handle_interrupts(interrupt_type interrupt) {
        if (interrupt == none)
            return;

        disable_interrupts();

        // 1 cycle
        run_phantom_cycle();
        // 3 cycles
        word interrupt_handler_address = interrupt_jump_targets[interrupt];
        call(interrupt_handler_address, true);
        // 1 cycle
        prefetch_next_instruction();
    }

    void cpu::request_interrupt(interrupt_type interrupt) {
        interrupt_requested_register = utility::set_bit(interrupt_requested_register, interrupt);
    }

    void cpu::acknowledge_interrupt(interrupt_type interrupt) {
        interrupt_requested_register = utility::clear_bit(interrupt_requested_register, interrupt);
    }
}