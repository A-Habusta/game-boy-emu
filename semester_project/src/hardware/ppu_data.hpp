// File: ppu_extras.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_PPU_DATA_HPP
#define SEMESTER_PROJECT_PPU_DATA_HPP

#include "../utility.hpp"

namespace ppu {
    // These struct act as a wrapper for raw VRAM/OAM data to make it easier to work with
    // A palette is basically a conversion table between 2-bit internal game boy colors and colors we draw to the
    // screen
    struct palette {
        using real_pixel_type = uint32_t;

        struct pixel {
            pixel(byte value) : value(value & internal_color_mask) {}
            byte get_value() const { return value; }

        private:
            byte value;
        };

        real_pixel_type convert_to_real_pixel(pixel pixel) const {
            return master_palette[get_color_index(pixel)];
        }
    private:
        static constexpr int colors_per_palette = 4;
        static constexpr int internal_color_mask = 0b11;
        static constexpr real_pixel_type master_palette[colors_per_palette] = {
                0xFF000000,
                0xFF555555,
                0xFFAAAAAA,
                0xFFFFFFFF
        };
        // The palettes are actualy indices into a master palette, every index is 2 bits
        byte colors;

        byte get_color_index(pixel pixel) const {
            byte palette_index = pixel.get_value();
            byte result = 0;

            result |= utility::get_bit(colors, palette_index * 2);
            result |= utility::get_bit(colors, palette_index * 2 + 1) << 1;

            return result;
        }
    };


    struct tile {
        constexpr static int size = 8;
        constexpr static int bytes_per_row = 2;

        palette::pixel get_pixel(int x, int y) const {
            x %= size;
            y %= size;

            byte low_bit_row = data[y * bytes_per_row];
            byte high_bit_row = data[y * bytes_per_row + 1];

            byte pixel = 0;
            pixel |= utility::get_bit(low_bit_row, 7 - x);
            pixel |= utility::get_bit(high_bit_row, 7 - x) << 1;

            return {pixel};
        }

    private:
        byte data[size * bytes_per_row];
    };

    struct tile_data {
        struct index {
            index() = default;
            byte get_value() const { return value; }

        private:
            byte value;
        };

        struct map {
            tile_data::index get_tile_index(int x, int y) const {
                x %= width;
                y %= height;

                return data[y][x];
            }
        private:
            static constexpr int height = 32;
            static constexpr int width = 32;

            tile_data::index data[height][width];
        };

        tile get_tile_oam(index index) const {
            return get_tile_method_8000(index.get_value());
        }

        tile get_tile_bg_and_window(index index, bool method) const {
            byte value = index.get_value();
            return method ? get_tile_method_8800(value) : get_tile_method_8000(value);
        }

    private:
        static constexpr int tile_count = 384;
        static constexpr int tile_offset_method_8800 = 128;

        tile tiles[tile_count];

        tile get_tile_method_8000(byte index) const {
            return tiles[index];
        }

        tile get_tile_method_8800(byte index) const {
            int8_t signed_index = (int8_t)(index);
            return tiles[tile_offset_method_8800 + signed_index];
        }
    };

    union vram_struct {
    private:
        static constexpr int size = 0x2000;
    public:
        struct {
            tile_data tiles;
            tile_data::map tile_map_0;
            tile_data::map tile_map_1;
        } tiles;
        byte raw_data[size]{};
    };

    struct sprite {
        bool get_priority() const { return get_attribute(priority_flag_position); }
        bool get_y_flip() const { return get_attribute(y_flip_flag_position); }
        bool get_x_flip() const { return get_attribute(x_flip_flag_position); }
        // There are only two palettes, so we can use a single bit
        bool get_palette_number() const { return get_attribute(palette_number_position); }

        // I don't think the raw values are ever used, so we can just return the corrected ones
        byte get_corrected_x() const { return x - x_offset; }
        byte get_corrected_y() const { return y - y_offset; }
        tile_data::index get_tile_number() const { return tile; }

        enum size {
            size8x8 = 0,
            size8x16 = 1
        };

    private:
        static constexpr int x_offset = 8;
        static constexpr int y_offset = 16;

        // data can be private, since the PPU never writes to this
        byte y;
        byte x;
        tile_data::index tile;
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

    struct register_file {
        byte lcd_control;
        byte lcd_status;
        byte scroll_y;
        byte scroll_x;
        byte lcd_y;
        byte lcd_y_compare;
        byte dma_transfer;
        palette background_palette;
        palette object_palette_0;
        palette object_palette_1;
        byte window_y;
        byte window_x;

        bool get_lcd_display_enable() const { return get_lcd_control_flag(lcd_display_enable); }
        bool get_window_tile_map_select() const { return get_lcd_control_flag(window_tile_map_select); }
        bool get_window_draw_enable() const { return get_lcd_control_flag(window_draw_enable); }
        bool get_bg_and_window_tile_data_select() const { return get_lcd_control_flag(bg_and_window_tile_data_select); }
        bool get_bg_tile_map_select() const { return get_lcd_control_flag(bg_tile_map_select); }
        bool get_obj_draw_enable() const { return get_lcd_control_flag(obj_draw_enable); }
        bool get_bg_window_display_priority() const { return get_lcd_control_flag(bg_window_display_priority); }
        sprite::size get_obj_size() const {
            return get_lcd_control_flag(obj_size) ? sprite::size::size8x16 : sprite::size::size8x8;
        }

        bool get_coincidence_interrupt_enable() const { return get_lcd_status_flag(coincidence_interrupt_enable); }
        bool get_oam_interrupt_enable() const { return get_lcd_status_flag(oam_interrupt_enable); }
        bool get_v_blank_interrupt_enable() const { return get_lcd_status_flag(v_blank_interrupt_enable); }
        bool get_h_blank_interrupt_enable() const { return get_lcd_status_flag(h_blank_interrupt_enable); }
        bool get_coincidence_flag() const { return get_lcd_status_flag(coincidence_flag); }

        void set_coincidence_flag(bool value) { write_lcd_status_flag(coincidence_flag, value); }

    private:
        enum lcd_control_flags {
            lcd_display_enable = 7,
            window_tile_map_select = 6,
            window_draw_enable = 5,
            bg_and_window_tile_data_select = 4,
            bg_tile_map_select = 3,
            obj_size = 2,
            obj_draw_enable = 1,
            bg_window_display_priority = 0
        };

        bool get_lcd_control_flag(lcd_control_flags flag) const {
            return utility::get_bit(lcd_control, flag);
        }

        enum lcd_status_flags {
            coincidence_interrupt_enable = 6,
            oam_interrupt_enable = 5,
            v_blank_interrupt_enable = 4,
            h_blank_interrupt_enable = 3,
            coincidence_flag = 2,
        };

        bool get_lcd_status_flag(lcd_status_flags flag) const {
            return utility::get_bit(lcd_status, flag);
        }

        void write_lcd_status_flag(lcd_status_flags flag, bool value) {
            utility::write_bit(lcd_status, flag, value);
        }
    };

    union oam_struct {
    private:
        static constexpr int sprite_count = 40;
        static constexpr int size = sprite_count * sizeof(sprite);
    public:
        sprite sprites[sprite_count];
        byte raw_data[size]{};
    };
}
#endif //SEMESTER_PROJECT_PPU_DATA_HPP