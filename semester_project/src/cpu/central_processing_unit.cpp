// File: central_processing_unit.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"

#include <utility>

#include <utility>
#include "../utility.hpp"

namespace central_processing_unit {
    cpu::cpu(memory_read_callback&& read_memory, memory_write_callback&& write_memory,
             cycle_callback&& run_phantom_cycle)
        : read_memory(std::move(read_memory)), write_memory(std::move(write_memory)),
          run_phantom_cycle(std::move(run_phantom_cycle)) {
    };

    void cpu::execute() {
        switch (current_state) {
            case state::running: execute_running_state(); break;
            case state::halted: execute_halted_state(); break;
            case state::crashed: execute_crashed_state(); break;

            // Can't occur
            default: break;
        }
    }

    void cpu::execute_running_state() {
        execute_instruction(cached_instruction);
    }

    void cpu::execute_halted_state() {
        if (check_for_interrupts() != none) {
            current_state = state::running;

            prefetch_next_instruction_and_handle_interrupts();
            return;
        }

        run_phantom_cycle();
    }

    void cpu::execute_crashed_state() {
        // CPU is dead, other components continue to function
        run_phantom_cycle();
    }

    void cpu::prefetch_next_instruction_and_handle_interrupts() {
        fetch_instruction();

        interrupt_type interrupt = check_for_interrupts();
        if (interrupt_master_enable && interrupt != none){
            handle_interrupts(interrupt);
        }
        else {
            inc_pc();
        }
    }

    cpu::interrupt_type cpu::check_for_interrupts() const {
        // Interrupts are serviced with priority going from interrupt in bit 0 -> bit 4
        for (int i = 0; i < interrupt_types_count; ++i) {
            bool interrupt_requested = utility::get_bit(interrupt_requested_register, i);
            bool interrupt_enabled = utility::get_bit(interrupt_enable_register, i);

            if (interrupt_requested && interrupt_enabled) {
                auto interrupt = static_cast<interrupt_type>(i);
                return interrupt;
            }
        }

        return none;
    }

    void cpu::handle_interrupts(interrupt_type interrupt) {
        if (interrupt == none)
            return;

        acknowledge_interrupt(interrupt);

        interrupt_master_enable = false;
        word interrupt_handler_address = interrupt_jump_targets[interrupt];

        // 3 cycles
        push_word_to_stack(registers.whole.PC);

        registers.whole.PC = interrupt_handler_address;
        // 1 cycle
        run_phantom_cycle();
        // 1 cycle
        fetch_instruction_and_increment_pc();
    }

    void cpu::request_interrupt(interrupt_type interrupt) {
        interrupt_requested_register = utility::set_bit(interrupt_requested_register, interrupt);
    }

    void cpu::acknowledge_interrupt(interrupt_type interrupt) {
        interrupt_requested_register = utility::clear_bit(interrupt_requested_register, interrupt);
    }
}