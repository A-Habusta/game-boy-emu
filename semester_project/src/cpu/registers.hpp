// File: registers.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_REGISTERS_HPP
#define SEMESTER_PROJECT_REGISTERS_HPP

#include <bit>
#include <concepts>
#include "../utility.hpp"

namespace registers {
    constexpr int FLAG_ZERO = 7;
    constexpr int FLAG_SUBTRACT = 6;
    constexpr int FLAG_HALF_CARRY = 5;
    constexpr int FLAG_CARRY = 4;

    enum class whole_register_name {
        AF, BC, DE, HL, SP, PC
    };

    enum class half_register_name {
        A, F, B, C, D, E, H, L
    };


    struct whole_registers {
        word AF;
        word BC;
        word DE;
        word HL;

        word SP;
        word PC;
    };

    struct half_registers_little_endian {
        byte F, A;
        byte C, B;
        byte E, D;
        byte L, H;
    };

    struct half_registers_big_endian {
        byte A, F;
        byte B, C;
        byte D, E;
        byte H, L;
    };

    // Does not work in mixed endianness
    constexpr bool is_little_endian = std::endian::native == std::endian::little;
    using half_registers_correct_endian = std::conditional_t<is_little_endian, half_registers_little_endian,
            half_registers_big_endian>;

    union register_file {
        whole_registers whole{};
        half_registers_correct_endian half;

        void write_zero_flag(bool value) { half.F = utility::write_bit(half.F, FLAG_ZERO, value); }
        void write_subtract_flag(bool value) { half.F = utility::write_bit(half.F, FLAG_SUBTRACT, value); }
        void write_half_carry_flag(bool value) { half.F = utility::write_bit(half.F, FLAG_HALF_CARRY, value); }
        void write_carry_flag(bool value) { half.F = utility::write_bit(half.F, FLAG_CARRY, value); }

        bool read_zero_flag() { return utility::get_bit(half.F, FLAG_ZERO); }
        bool read_subtract_flag() { return utility::get_bit(half.F, FLAG_SUBTRACT); }
        bool read_half_carry_flag() { return utility::get_bit(half.F, FLAG_HALF_CARRY); }
        bool read_carry_flag() { return utility::get_bit(half.F, FLAG_CARRY); }

        void write_to_register(half_register_name register_name, byte value);
        void write_to_register(whole_register_name  register_name, word value);
        byte read_from_register(half_register_name register_name);
        word read_from_register(whole_register_name register_name);
    };
}


#endif //SEMESTER_PROJECT_REGISTERS_HPP