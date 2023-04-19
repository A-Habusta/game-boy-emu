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
    };
}


#endif //SEMESTER_PROJECT_REGISTERS_HPP
