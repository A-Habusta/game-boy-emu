// File: utility.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_UTILITY_HPP
#define SEMESTER_PROJECT_UTILITY_HPP

#include <string_view>
#include <functional>
#include <fstream>
#include <cstdint>

using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;

namespace utility {
    constexpr byte undefined_byte = 0xFF;

    template<typename T>
    inline constexpr T set_bit(T value, int bit) {
        return value | (1 << bit);
    }

    template<typename T>
    inline constexpr T clear_bit(T value, int bit) {
        return value & ~(1 << bit);
    }

    template<typename T>
    inline constexpr T write_bit(T value, int bit, bool bit_value) {
        return bit_value ? set_bit(value, bit) : clear_bit(value, bit);
    }

    template<typename T>
    inline constexpr bool get_bit(T value, int bit) {
        return (value >> bit) & 1;
    }

    inline constexpr bool get_lower_nibble(byte value) {
        return value & 0x0F;
    }

    inline constexpr bool get_higher_nibble(byte value) {
        return value >> 4;
    }

    // Cast truncates the value, no need to mask
    inline byte get_low_byte(word value) {
        return value;
    }

    inline byte get_high_byte(word value) {
        return value >> 8;
    }

    inline word get_word_from_bytes(byte low, byte high) {
        return (((word)high) << 8) | (word) low;
    }

    inline void read_file(std::string_view path, byte* target, std::size_t size, std::string_view error = "") {
        std::ifstream file(path.data(), std::ios::binary);
        if (!file.is_open()) {
            if (!error.empty())
                throw std::runtime_error(std::string(error) + " " + path.data());
            else
                throw std::runtime_error(path.data());
        }

        file.read(reinterpret_cast<char*>(target), size);
        file.close();
    }

    inline word sign_extend_byte_to_word(byte lower_byte) {
        byte upper_byte = 0;
        if (get_bit(lower_byte, 7)) {
            upper_byte = 0xFF;
        }

        return get_word_from_bytes(lower_byte, upper_byte);
    }
}

#endif //SEMESTER_PROJECT_UTILITY_HPP
