// File: cpu_execute_methods.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../emulator.hpp"
#include "../utility.hpp"

namespace central_processing_unit {
    void cpu::load(registers::half_register_name target_register, byte value) {
        registers.write_to_register(target_register, value);
        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::load(registers::whole_register_name target_register, word value) {
        registers.write_to_register(target_register, value);
        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::load(word target_address, byte value) {
        write_byte(target_address, value);
        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::load_sp_plus_imm_to_hl() {
        byte imm = read_byte_at_pc_and_increment();
        word result = add_signed_to_sp(imm);

        // This has prefetch in it
        load(registers::whole_register_name::HL, result);
    }

    void cpu::pop(registers::whole_register_name target_register) {
        registers.write_to_register(target_register, pop_word_from_stack());
        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::push(word value) {
        push_word_to_stack(value);
        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::add(byte value, bool carry) {
        byte result = registers.half.A + value + carry;

        registers.half.A = result;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag((((registers.half.A & 0xF) + (value & 0xF) + carry) & 0x10) != 0);
        registers.write_carry_flag(((word)registers.half.A + (word)value + carry) > 0xFF);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::add16(word value) {
        word result = registers.whole.HL + value;

        registers.whole.HL = result;

        registers.write_subtract_flag(false);
        registers.write_half_carry_flag((((registers.whole.HL & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0);
        registers.write_carry_flag((dword)registers.whole.HL + (dword)value > 0xFFFF);

        run_phantom_cycle();

        prefetch_next_instruction_and_handle_interrupts();
    }

    word cpu::add_signed_to_sp(byte value) {
        word sign_extended_value = utility::sign_extend_byte_to_word(value);
        word result = registers.whole.SP + sign_extended_value;

        registers.write_zero_flag(false);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag((((registers.whole.HL & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0);
        registers.write_carry_flag((dword)registers.whole.HL + (dword)value > 0xFFFF);

        // Not sure if correct order but nothing can access SP (I think) during this cycle anyway
        run_phantom_cycle();
        return result;
    }

    void cpu::sp_plus_signed_imm() {
        byte imm = read_byte_at_pc_and_increment();
        word result = add_signed_to_sp(imm);

        registers.whole.SP = result;
        run_phantom_cycle();

        prefetch_next_instruction_and_handle_interrupts();
    }

    byte cpu::subtract(byte value, bool carry) {
        byte result = registers.half.A - value - carry;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(true);
        registers.write_half_carry_flag((((registers.half.A & 0xF) - (value & 0xF) - carry) & 0x10) != 0);
        registers.write_carry_flag((word)registers.half.A < ((word)value + carry));

        return result;
        // No prefetch, this is a shared instruction
    }

    void cpu::sub(byte value) {
        registers.half.A = subtract(value, false);
        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::sbc(byte value) {
        registers.half.A = subtract(value, registers.read_carry_flag());
        prefetch_next_instruction_and_handle_interrupts();
    }


    void cpu::and_(byte value) {
        registers.half.A &= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(true);
        registers.write_carry_flag(false);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::or_(byte value) {
        registers.half.A |= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag(false);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::xor_(byte value) {
        registers.half.A ^= value;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag(false);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::cp(byte value) {
        // We don't want to change the value of A, only the flags
        subtract(value, false);

        prefetch_next_instruction_and_handle_interrupts();
    }


    byte cpu::shared_inc_dec(byte value, byte offset) {
        byte result = value + offset;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag((((value & 0xF) + offset) & 0x10) != 0);

        return result;
    }

    void cpu::inc(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        byte result = shared_inc_dec(value, 1);
        registers.write_to_register(target_register, result);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::inc(word target_address) {
        byte value = read_byte(target_address);

        byte result = shared_inc_dec(value, 1);
        write_byte(target_address, result);

        prefetch_next_instruction_and_handle_interrupts();
    }

    // for some reason this does not affect any flags
    void cpu::inc(registers::whole_register_name target_register) {
        word value = registers.read_from_register(target_register);

        word result = value + 1;
        registers.write_to_register(target_register, result);

        run_phantom_cycle();

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::dec(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        byte result = shared_inc_dec(value, -1);
        registers.write_to_register(target_register, result);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::dec(word target_address) {
        byte value = read_byte(target_address);

        byte result = shared_inc_dec(value, -1);
        write_byte(target_address, result);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::dec(registers::whole_register_name target_register) {
        word value = registers.read_from_register(target_register);

        word result = value - 1;
        registers.write_to_register(target_register, result);

        run_phantom_cycle();

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::set_carry_flag(bool value) {
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag(value);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::complement_A() {
        registers.half.A = ~registers.half.A;
        registers.write_subtract_flag(true);
        registers.write_half_carry_flag(true);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::daa() {
        bool carry = false;
        byte correction = 0;

        if (!registers.read_subtract_flag()) {
            if (registers.read_carry_flag() || registers.half.A > 0x99) {
                carry = true;
                correction |= 0x60;
            }
            if (registers.read_half_carry_flag() || (registers.half.A & 0x0F) > 0x09) {
                correction |= 0x06;
            }
        }
        else if (registers.read_carry_flag()) {
            carry = true;
            correction = registers.read_half_carry_flag() ? 0x9A : 0xA0;
        }
        else if (registers.read_half_carry_flag()) {
            correction = 0xFA;
        }

        registers.half.A += correction;

        registers.write_zero_flag(registers.half.A == 0);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag(carry);

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::jump_hl() {
        registers.whole.PC = registers.whole.HL;

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::jump(word address, bool condition) {
        if (condition) {
            run_phantom_cycle();
            registers.whole.PC = address;
        }

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::jump_relative(byte offset, bool condition) {
        if (condition) {
            word sign_extended_offset = utility::sign_extend_byte_to_word(offset);
            word result = registers.whole.PC + sign_extended_offset;

            run_phantom_cycle();
            registers.whole.PC = result;
        }

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::call(word address, bool condition) {
        if (condition) {
            push_word_to_stack(registers.whole.PC);
            registers.whole.PC = address;
        }

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::return_always() {
        registers.whole.PC = pop_word_from_stack();
        run_phantom_cycle();

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::return_from_interrupt() {
        registers.whole.PC = pop_word_from_stack();
        run_phantom_cycle();
        interrupt_master_enable = true;

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::return_conditional(bool condition) {
        run_phantom_cycle();
        if (condition) {
            registers.whole.PC = pop_word_from_stack();
            run_phantom_cycle();
        }

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::enable_interrupts_instruction() {
        // Order is important here, as the interrupt master enable flag is set to true
        prefetch_next_instruction_and_handle_interrupts();

        interrupt_master_enable = true;
    }

    void cpu::disable_interrupts_instruction() {
        interrupt_master_enable = false;

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::stop() {
        // Not sure if this is correct
        fetch_instruction();
        if (cached_instruction != 0x00) {
            crash();
        }
        throw emulator::stop{};
    }
    void cpu::halt() {
        fetch_instruction();

        auto interrupt = check_for_interrupts();
        if (interrupt != none) {
            if (interrupt_master_enable) {
                handle_interrupts(interrupt);
            }
            return;
        }

        run_phantom_cycle();
        current_state = state::halted;
    }

    void cpu::handle_unknown_instruction() {
        crash();
    }

    byte cpu::shared_shift_in_value_left(byte value, bool bit) {
        byte result = value << 1 | bit;

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag((value & 0x80) != 0);

        return result;
    }

    byte cpu::shared_shift_in_value_right(byte value, bool bit) {
        byte result = value >> 1 | (bit << 7);

        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag((value & 0x01) != 0);

        return result;
    }

    void cpu::rotate_a_left(bool added_bit) {
        byte result = shared_shift_in_value_left(registers.half.A, added_bit);
        registers.write_zero_flag(false);
        registers.half.A = result;

        prefetch_next_instruction_and_handle_interrupts();
    }

    void cpu::rotate_a_right(bool added_bit) {
        byte result = shared_shift_in_value_right(registers.half.A, added_bit);
        registers.write_zero_flag(false);
        registers.half.A = result;

        prefetch_next_instruction_and_handle_interrupts();
    }


    void cpu::rlc(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = utility::get_bit(value, 7);
        byte result = shared_shift_in_value_left(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::rlc(word address) {
        byte value = read_byte(address);

        bool added_bit = utility::get_bit(value, 7);
        byte result = shared_shift_in_value_left(value, added_bit);

        write_byte(address, result);
    }


    void cpu::rl(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = registers.read_carry_flag();
        byte result = shared_shift_in_value_left(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::rl(word address) {
        byte value = read_byte(address);

        bool added_bit = registers.read_carry_flag();
        byte result = shared_shift_in_value_left(value, added_bit);

        write_byte(address, result);
    }


    void cpu::rrc(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = utility::get_bit(value, 0);
        byte result = shared_shift_in_value_right(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::rrc(word address) {
        byte value = read_byte(address);

        bool added_bit = utility::get_bit(value, 0);
        byte result = shared_shift_in_value_right(value, added_bit);

        write_byte(address, result);
    }


    void cpu::rr(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = registers.read_carry_flag();
        byte result = shared_shift_in_value_right(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::rr(word address) {
        byte value = read_byte(address);

        bool added_bit = registers.read_carry_flag();
        byte result = shared_shift_in_value_right(value, added_bit);

        write_byte(address, result);
    }

    void cpu::sla(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = false;
        byte result = shared_shift_in_value_left(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::sla(word address) {
        byte value = read_byte(address);

        bool added_bit = false;
        byte result = shared_shift_in_value_left(value, added_bit);

        write_byte(address, result);
    }


    void cpu::sra(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = utility::get_bit(value, 7);
        byte result = shared_shift_in_value_right(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::sra(word address) {
        byte value = read_byte(address);

        bool added_bit = utility::get_bit(value, 7);
        byte result = shared_shift_in_value_right(value, added_bit);

        write_byte(address, result);
    }


    void cpu::srl(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        bool added_bit = false;
        byte result = shared_shift_in_value_right(value, added_bit);

        registers.write_to_register(target_register, result);
    }

    void cpu::srl(word address) {
        byte value = read_byte(address);

        bool added_bit = false;
        byte result = shared_shift_in_value_right(value, added_bit);

        write_byte(address, result);
    }

    void cpu::swap(registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        byte result = (value << 4) | (value >> 4);
        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag(false);

        registers.write_to_register(target_register, result);
    }

    void cpu::swap(word address) {
        byte value = read_byte(address);

        byte result = (value << 4) | (value >> 4);
        registers.write_zero_flag(result == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(false);
        registers.write_carry_flag(false);

        write_byte(address, result);
    }

    void cpu::bit(int bit, registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);

        registers.write_zero_flag(utility::get_bit(value, bit) == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(true);
    }

    void cpu::bit(int bit, word address) {
        byte value = read_byte(address);

        registers.write_zero_flag(utility::get_bit(value, bit) == 0);
        registers.write_subtract_flag(false);
        registers.write_half_carry_flag(true);
    }

    void cpu::set(int bit, registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);
        value = utility::set_bit(value, bit);
        registers.write_to_register(target_register, value);
    }

    void cpu::set(int bit, word address) {
        byte value = read_byte(address);
        value = utility::set_bit(value, bit);
        write_byte(address, value);
    }

    void cpu::res(int bit, registers::half_register_name target_register) {
        byte value = registers.read_from_register(target_register);
        value = utility::clear_bit(value, bit);
        registers.write_to_register(target_register, value);
    }

    void cpu::res(int bit, word address) {
        byte value = read_byte(address);
        value = utility::clear_bit(value, bit);
        write_byte(address, value);
    }
}