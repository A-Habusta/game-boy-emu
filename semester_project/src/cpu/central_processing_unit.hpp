// File: central_processing_unit.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP
#define SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP

#include "../memory.hpp"
#include "../utility.hpp"
#include "registers.hpp"

// TODO: actually create this
class emulator {};
struct emulatorDelegate {
    emulatorDelegate(emulator& emulator) : emu(emulator) {}
    void operator()() { (emu.*method)(); }
private:
    using emulatorMethod = void(emulator::*)();
    emulator& emu;
    emulatorMethod method;
};

namespace central_processing_unit {

    constexpr int HIGH_PAGE = 0xFF00;


    class cpu {
    public:
        void reset();
        void execute_instruction();
    private:
        registers::register_file registers;
        memory::memory_map memory;
        byte cached_instruction;

        emulatorDelegate run_phantom_cycle;

        void prefetch_next_instruction();

        //
        // Memory access methods
        //
        byte read_byte(word address) { return memory.read_with_cycling(address); }
        word read_word(word address) {
            return utility::get_word_from_byte(read_byte(address), read_byte(address + 1));
        }

        byte read_byte_at_pc_with_increment() { return read_byte(registers.whole.PC++); }
        // We don't use read_word here so PC is incremented after each byte read
        word read_word_at_pc_with_increment() {
            byte low = read_byte_at_pc_with_increment();
            byte high = read_byte_at_pc_with_increment();
            return utility::get_word_from_byte(low, high);
        }

        void write_byte_to_memory(word address, byte value) { memory.write_with_cycling(address, value); }
        void write_word_to_memory(word address, word value) {
            write_byte_to_memory(address, utility::get_low_byte(value));
            write_byte_to_memory(address + 1, utility::get_high_byte(value));
        }

        void push_byte_to_stack(byte value) { write_byte_to_memory(--registers.whole.SP, value); }
        void push_word_to_stack(word value) {
            push_byte_to_stack(utility::get_high_byte(value));
            push_byte_to_stack(utility::get_low_byte(value));
        }

        byte pop_byte_from_stack() { return read_byte(registers.whole.SP++); }
        word pop_word_from_stack() {
            byte low = pop_byte_from_stack();
            byte high = pop_byte_from_stack();
            return utility::get_word_from_byte(low, high);
        }
        //
        // End of memory access methods
        //


        // Instructions
        void load(registers::half_register_name target_register, byte value);
        void load(registers::whole_register_name target_register, word value);
        void load(word target_address, byte value);

        void pop(registers::whole_register_name target_register);
        void push(word value);
    };

}

#endif //SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP