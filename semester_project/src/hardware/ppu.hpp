// File: ppu.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_PPU_HPP
#define SEMESTER_PROJECT_PPU_HPP

#include "../utility.hpp"

namespace ppu {
    namespace {
        struct sprite {
            bool get_priority() const { return get_attribute(priority_flag_position); }
            bool get_y_flip() const { return get_attribute(y_flip_flag_position); }
            bool get_x_flip() const { return get_attribute(x_flip_flag_position); }
            // There are only two palettes, so we can use a single bit
            bool get_palette_number() const { return get_attribute(palette_number_position); }

            enum size {
                size8x8 = 0,
                size8x16 = 1
            };

        private:
            static constexpr int x_offset = 8;
            static constexpr int y_offset = 16;

            // data can be private, since the PPU only reads this
            byte y;
            byte x;
            byte tile;
            byte attributes;

            enum {
                priority_flag_position = 7,
                y_flip_flag_position = 6,
                x_flip_flag_position = 5,
                palette_number_position = 4,
            };

            bool get_attribute(int flag_position) const {
                return utility::get_bit(attributes, flag_position);
            }
        };

        struct tile {

        };
    }
}

#endif //SEMESTER_PROJECT_PPU_HPP
