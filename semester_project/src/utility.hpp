// File: utility.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_UTILITY_HPP
#define SEMESTER_PROJECT_UTILITY_HPP


#include <cstdint>

using byte = uint8_t;
using word = uint16_t;

namespace utility {
    template<typename T>
    constexpr T set_bit(T value, int bit) {
        return value | (1 << bit);
    }

    template<typename T>
    constexpr T clear_bit(T value, int bit) {
        return value & ~(1 << bit);
    }

    template<typename T>
    constexpr T write_bit(T value, int bit, bool bit_value) {
        return bit_value ? set_bit(value, bit) : clear_bit(value, bit);
    }

    template<typename T>
    constexpr T get_bit(T value, int bit) {
        return (value >> bit) & 1;
    }

    // Cast truncates the value, no need to mask
    byte get_low_byte(word value) {
        return value;
    }

    byte get_high_byte(word value) {
        return value >> 8;
    }

    word get_word_from_byte(byte low, byte high) {
        return (((word)high) << 8) | low;
    }
}

#endif //SEMESTER_PROJECT_UTILITY_HPP
