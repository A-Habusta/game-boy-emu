// File: central_processing_unit.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP
#define SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP

#include <functional>

#include "../utility.hpp"
#include "registers.hpp"

namespace central_processing_unit {
    constexpr int high_page = 0xFF00;
    constexpr word interrupt_jump_targets[] = { 0x40, 0x48, 0x50, 0x58, 0x60 };
    constexpr int interrupt_types_count = 5;

    class cpu {
        using memory_read_callback = std::function<byte(word)>;
        using memory_write_callback = std::function<void(word, byte)>;
        using cycle_callback = std::function<void()>;

    public:
        cpu(memory_read_callback read_memory, memory_write_callback write_memory, cycle_callback run_phantom_cycle);
        void execute();

        byte interrupt_enable_register{};
        byte interrupt_requested_register{};

        void request_v_blank_interrupt() { request_interrupt(interrupt_type::vblank); }
        void request_lcd_stat_interrupt() { request_interrupt(interrupt_type::lcd_stat); }
        void request_timer_interrupt() { request_interrupt(interrupt_type::timer); }
        void request_serial_interrupt() { request_interrupt(interrupt_type::serial); }
        void request_joypad_interrupt() { request_interrupt(interrupt_type::joypad); }

    private:

        registers::register_file registers{};
        byte cached_instruction{0x00};

        memory_read_callback read_memory;
        memory_write_callback write_memory;

        cycle_callback run_phantom_cycle;
        bool interrupt_master_enable = true;

        enum class state {
            running,
            halted,
            crashed
        };

        state current_state = state::running;

        void crash() { current_state = state::crashed; };

        void execute_running_state();
        void execute_halted_state();
        void execute_crashed_state();

        void execute_instruction(byte instruction);
        void prefetch_next_instruction_and_handle_interrupts();

        // Not class so that we can easily use it as a number
        enum interrupt_type {
            none = -1,
            vblank = 0,
            lcd_stat = 1,
            timer = 2,
            serial = 3,
            joypad = 4,
        };

        interrupt_type check_for_interrupts() const;
        void acknowledge_interrupt(interrupt_type);
        void handle_interrupts(interrupt_type);
        void request_interrupt(interrupt_type);
        //
        // Memory access methods
        //
        byte read_byte(word address) { return read_memory(address); }

        byte read_byte_at_pc_and_increment() {
            byte val = read_byte_at_pc();
            inc_pc();
            return val;
        }

        // We don't use read_word here so PC is incremented after each byte read
        word read_word_at_pc_and_increment() {
            byte low = read_byte_at_pc_and_increment();
            byte high = read_byte_at_pc_and_increment();
            return utility::get_word_from_bytes(low, high);
        }

        byte read_byte_at_pc() { return read_byte(registers.whole.PC); }

        void write_byte(word address, byte value) { write_memory(address, value); }

        void push_byte_to_stack(byte value) { write_byte(--registers.whole.SP, value); }
        void push_word_to_stack(word value) {
            run_phantom_cycle();
            push_byte_to_stack(utility::get_high_byte(value));
            push_byte_to_stack(utility::get_low_byte(value));
        }

        byte pop_byte_from_stack() { return read_byte(registers.whole.SP++); }
        word pop_word_from_stack() {
            byte low = pop_byte_from_stack();
            byte high = pop_byte_from_stack();
            return utility::get_word_from_bytes(low, high);
        }

        void fetch_instruction() { cached_instruction = read_byte_at_pc(); }
        void fetch_instruction_and_increment_pc() {
            fetch_instruction();
            inc_pc();
        }

        void inc_pc() { registers.whole.PC++; }

        //
        // End of memory access methods
        //

        // Instructions
        void load(registers::half_register_name target_register, byte value);
        void load(registers::whole_register_name target_register, word value);
        void load(word target_address, byte value);

        void load_sp_plus_imm_to_hl();

        void pop(registers::whole_register_name target_register);
        void push(word value);

        // Always adds value to A
        void add(byte value, bool carry = false);

        byte subtract(byte value, bool carry);

        void sub(byte value);
        void sbc(byte value);

        // Always adds value to HL
        void add16(word value);
        word add_signed_to_sp(byte value);

        void sp_plus_signed_imm();

        void and_(byte value);
        void or_(byte value);
        void xor_(byte value);

        void cp(byte value);

        byte shared_inc_dec(byte value, byte offset);
        void inc(registers::half_register_name target_register);
        void inc(word target_address);
        void inc(registers::whole_register_name target_register);

        void dec(registers::half_register_name target_register);
        void dec(word target_address);
        void dec(registers::whole_register_name target_register);

        void set_carry_flag(bool value);
        void complement_A();
        void daa();

        // These are separated because the timing is different
        void jump_hl();
        void jump(word target_address, bool condition);

        void jump_relative(byte offset, bool condition);

        void call(word target_address, bool condition);

        void return_always();
        void return_from_interrupt();
        void return_conditional(bool condition);

        void stop();
        void halt();

        void enable_interrupts_instruction();
        void disable_interrupts_instruction();

        void execute_cb_prefixed_instruction();

        void handle_unknown_instruction();

        //CB instructions and bitwise shifts

        byte shared_shift_in_value_left(byte value, bool bit);
        byte shared_shift_in_value_right(byte value, bool bit);

        void rotate_a_right(bool added_bit);
        void rotate_a_left(bool added_bit);

        void rlc(registers::half_register_name target_register);
        void rlc(word target_address);

        void rl(registers::half_register_name target_register);
        void rl(word target_address);

        void rrc(registers::half_register_name target_register);
        void rrc(word target_address);

        void rr(registers::half_register_name target_register);
        void rr(word target_address);

        void sla(registers::half_register_name target_register);
        void sla(word target_address);

        void sra(registers::half_register_name target_register);
        void sra(word target_address);

        void srl(registers::half_register_name target_register);
        void srl(word target_address);

        void swap(registers::half_register_name target_register);
        void swap(word target_address);

        void bit(int bit, registers::half_register_name target_register);
        void bit(int bit, word target_address);

        void set(int bit, registers::half_register_name target_register);
        void set(int bit, word target_address);

        void res(int bit, registers::half_register_name target_register);
        void res(int bit, word target_address);
    };
}

#endif //SEMESTER_PROJECT_CENTRAL_PROCESSING_UNIT_HPP