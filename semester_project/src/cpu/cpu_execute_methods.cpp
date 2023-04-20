// File: cpu_execute_methods.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../utility.hpp"

namespace central_processing_unit {
    void cpu::load(registers::half_register_name target_register, byte value) {
        registers.write_to_register(target_register, value);
    }

    void cpu::load(registers::whole_register_name target_register, word value) {
        registers.write_to_register(target_register, value);
    }

    void cpu::load(word target_address, byte value) {
        write_byte_to_memory(target_address, value);
    }

    void cpu::pop(registers::whole_register_name target_register) {
        registers.write_to_register(target_register, pop_word_from_stack());
    }

    void cpu::push(word value) {
        run_phantom_cycle();
        push_word_to_stack(value);
    }

    void cpu::add(byte value, bool carry) {
        byte result = registers.half.A + value + carry;

        registers.half.A = result;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag((registers.half.A & 0xF) + (value & 0xF) + carry & 0x10 != 0);
        registers.write_carry_flag((word)registers.half.A + (word)value + carry > 0xFF);
    }

    void cpu::add16(word value) {
        word result = registers.whole.HL + value;

        registers.whole.HL = result;

        registers.write_subtract_flag(0);
        registers.write_half_carry_flag((registers.whole.HL & 0xFFF) + (value & 0xFFF) & 0x1000 != 0);
        registers.write_carry_flag((dword)registers.whole.HL + (dword)value + carry > 0xFFFF);

        run_phantom_cycle();
    }

    void cpu::add_to_sp(byte value) {
        word result = registers.whole.SP + value;

        registers.write_zero_flag(0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag((registers.whole.HL & 0xFFF) + (value & 0xFFF) & 0x1000 != 0);
        registers.write_carry_flag((dword)registers.whole.HL + (dword)value + carry > 0xFFFF);

        // Not sure if correct order but nothing can access SP (i think) during this cycle anyway
        run_phantom_cycle();
        return result;
    }

    void cpu::sub(byte value, bool carry) {
        byte result = registers.half.A - value - carry;

        registers.half.A = result;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(1);
        registers.write_half_carry_flag((registers.half.A & 0xF) - (value & 0xF) - carry & 0x10 != 0);
        registers.write_carry_flag((word)registers.half.A - (word)value - carry > 0xFF);
    }

    void cpu::and_(byte value) {
        registers.half.A &= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag(1);
        registers.write_carry_flag(0);
    }

    void cpu::or_(byte value) {
        registers.half.A |= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag(0);
        registers.write_carry_flag(0);
    }

    void cpu::xor_(byte value) {
        registers.half.A ^= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag(0);
        registers.write_carry_flag(0);
    }

    void cpu::cp(byte value) {
        byte cache = registers.half.A;
        // We don't want to change the value of A, only the flags
        sub(value, 0);
        registers.half.A = cache;
    }


    byte cpu::shared_inc_dec(byte value, byte offset) {
        byte result = value + offset;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(0);
        registers.write_half_carry_flag((value & 0xF) + offset & 0x10 != 0);

        return result;
    }

    void cpu::inc(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        byte result = shared_inc_dec(value, 1);
        registers.write_to_register(target_register, result);
    }

    void cpu::inc(word target_address) {
        byte value = read_byte_from_memory(target_address);

        byte result = shared_inc_dec(value, 1);
        write_byte_to_memory(target_address, result);
    }

    // for some reason this does not affect any flags
    void cpu::inc(registers::whole_register_name target_register) {
        word value = registers.read_from_register(target_register);

        word result = value + 1;
        registers.write_to_register(target_register, result);

        run_phantom_cycle();
    }

    void cpu::dec(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        byte result = shared_inc_dec(value, -1);
        registers.write_to_register(target_register, result);
    }

    void cpu::dec(word target_address) {
        byte value = read_byte_from_memory(target_address);

        byte result = shared_inc_dec(value, -1);
        write_byte_to_memory(target_address, result);
    }

    void cpu::dec(registers::whole_register_name target_register) {
        word value = registers.read_from_register(target_register);

        word result = value - 1;
        registers.write_to_register(target_register, result);

        run_phantom_cycle();
    }
}