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

    void cpu::add(byte value, bool carry) {
        byte result = registers.half.A + value + carry;

        registers.half.A = result;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag((registers.half.A & 0xF) + (value & 0xF) + carry & 0x10 != 0);
        registers.write_carry_flag((word)registers.half.A + (word)value + carry > 0xFF);

        prefetch_next_instruction();
    }

    void cpu::sub(byte value, bool carry) {
        byte result = registers.half.A - value - carry;

        registers.half.A = result;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(true);
        registers.write_half_carry_flag((registers.half.A & 0xF) - (value & 0xF) - carry & 0x10 != 0);
        registers.write_carry_flag((word)registers.half.A - (word)value - carry > 0xFF);

        prefetch_next_instruction();
    }

    void cpu::and_(byte value) {
        registers.half.A &= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag(1);
        registers.write_carry_flag(0);

        prefetch_next_instruction();
    }

    void cpu::or_(byte value) {
        registers.half.A |= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag(0);
        registers.write_carry_flag(0);

        prefetch_next_instruction();
    }

    void cpu::xor_(byte value) {
        registers.half.A ^= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag(0);
        registers.write_carry_flag(0);

        prefetch_next_instruction();
    }

    void cpu::cp(byte value) {
        byte result = registers.half.A - value - carry;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(true);
        registers.write_half_carry_flag((registers.half.A & 0xF) - (value & 0xF) - carry & 0x10 != 0);
        registers.write_carry_flag((word)registers.half.A - (word)value - carry > 0xFF);

        prefetch_next_instruction();
    }

}