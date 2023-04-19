// File: central_processing_unit.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP
#define SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP

#include "../memory.hpp"
#include "../utility.hpp"
#include "registers.hpp"

namespace central_processing_unit {

    class cpu {
        registers::register_file registers;
        memory::memory_map memory;
        byte cached_instruction;

        void prefetch_next_instruction();

        byte read_byte_from_memory(word address) { return memory.read_with_cycling(address); }
        word read_word_from_memory(word address) {
            return utility::get_word_from_byte(read_byte_from_memory(address), read_byte_from_memory(address + 1));
        }

        byte read_byte_at_pc_with_increment() { return read_byte_from_memory(registers.whole.PC++); }
        word read_word_at_pc_with_increment() { return read_word_from_memory(registers.whole.PC++); }

        void write_byte_to_memory(word address, byte value) { memory.write_with_cycling(address, value); }
        void write_word_to_memory(word address, word value) {
            write_byte_to_memory(address, utility::get_low_byte(value));
            write_byte_to_memory(address + 1, utility::get_high_byte(value));
        }

    public:
        void reset();
        void execute_instruction();
    };
}

#endif //SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP