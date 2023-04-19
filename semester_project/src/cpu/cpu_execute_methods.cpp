// File: cpu_execute_methods.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../utility.hpp"

namespace central_processing_unit {
    void cpu::load(registers::half_register_name target_register, byte value) {
        registers.write_to_register(target_register, value);
        prefetch_next_instruction();
    }

    void cpu::load(registers::whole_register_name target_register, word value) {
        registers.write_to_register(target_register, value);
        prefetch_next_instruction();
    }

    void cpu::load(word target_address, byte value) {
        write_byte_to_memory(target_address, value);
        prefetch_next_instruction();
    }

    void cpu::pop(registers::whole_register_name target_register) {
        registers.write_to_register(target_register, pop_word_from_stack());
        prefetch_next_instruction();
    }

    void cpu::push(word value) {
        run_phantom_cycle();
        push_word_to_stack(value);
        prefetch_next_instruction();
    }
}