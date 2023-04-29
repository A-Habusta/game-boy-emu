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

        void write_zero_flag(bool value) { half.F = utility::write_bit(half.F, zero_flag_pos, value); }
        void write_subtract_flag(bool value) { half.F = utility::write_bit(half.F, subtract_flag_pos, value); }
        void write_half_carry_flag(bool value) { half.F = utility::write_bit(half.F, half_carry_flag_pos, value); }
        void write_carry_flag(bool value) { half.F = utility::write_bit(half.F, carry_flag_pos, value); }

        bool read_zero_flag() const { return utility::get_bit(half.F, zero_flag_pos); }
        bool read_subtract_flag() const { return utility::get_bit(half.F, subtract_flag_pos); }
        bool read_half_carry_flag() const { return utility::get_bit(half.F, half_carry_flag_pos); }
        bool read_carry_flag() const { return utility::get_bit(half.F, carry_flag_pos); }

        void write_to_register(half_register_name register_name, byte value);
        void write_to_register(whole_register_name  register_name, word value);
        byte read_from_register(half_register_name register_name) const;
        word read_from_register(whole_register_name register_name) const;
    private:
        enum {
            zero_flag_pos = 7,
            subtract_flag_pos = 6,
            half_carry_flag_pos = 5,
            carry_flag_pos = 4
        };
    };
}


#endif //SEMESTER_PROJECT_REGISTERS_HPP
