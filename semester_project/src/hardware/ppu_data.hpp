// File: ppu_extras.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_PPU_DATA_HPP
#define SEMESTER_PROJECT_PPU_DATA_HPP

#include <algorithm>
#include <optional>

#include "../utility.hpp"

namespace pixel_processing_unit {
    // These struct act as a wrapper for raw VRAM/OAM data to make it easier to work with
    // A palette is basically a conversion table between 2-bit internal game boy colors and colors we draw to the
    // screen
    struct palette {
        using real_pixel_type = uint32_t;

        struct pixel {
            pixel(byte value) : value(value & internal_color_mask) {}
            [[nodiscard]] byte get_value() const { return value; }
            [[nodiscard]] bool is_transparent() const { return value == 0; }

        private:
            byte value;
        };

        [[nodiscard]] real_pixel_type convert_to_real_pixel(pixel pixel) const {
            return master_palette[get_color_index(pixel)];
        }

        // To be used by outside I/O
        void write_raw_value(byte value) { colors = value; }
        [[nodiscard]] byte read_raw_value() const { return colors; }
    private:
        static constexpr int colors_per_palette = 4;
        static constexpr byte internal_color_mask = 0b11;
        static constexpr real_pixel_type master_palette[colors_per_palette] = {
                0xFFFFFFFF,
                0xFFAAAAAA,
                0xFF555555,
                0xFF000000
        };
        // The palettes are actualy indices into a master palette, every index is 2 bits
        byte colors;

        [[nodiscard]] byte get_color_index(pixel pixel) const {
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

        [[nodiscard]] palette::pixel get_pixel(int x, int y) const {
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
        struct map {
            [[nodiscard]] byte get_tile_index(int x, int y) const {
                x %= width;
                y %= height;

                return data[y][x];
            }
        private:
            static constexpr int height = 32;
            static constexpr int width = 32;

            byte data[height][width];
        };

        [[nodiscard]] tile get_tile_oam(byte index) const {
            return get_tile_method_8000(index);
        }

        [[nodiscard]] tile get_tile_bg_and_window(byte index, bool method) const {
            return method ? get_tile_method_8000(index) : get_tile_method_8800(index);
        }

    private:
        static constexpr int tile_count = 384;
        static constexpr word method_8800_base_index = 256;

        tile tiles[tile_count];

        [[nodiscard]] tile get_tile_method_8000(byte index) const {
            return tiles[index];
        }

        [[nodiscard]] tile get_tile_method_8800(byte index) const {
            word extended_offset = utility::sign_extend_byte_to_word(index);
            word result_index = method_8800_base_index + extended_offset;

            return tiles[result_index];
        }
    };

    struct sprite {
        [[nodiscard]] bool get_priority() const { return get_attribute(priority_flag_position); }
        [[nodiscard]] bool get_y_flip() const { return get_attribute(y_flip_flag_position); }
        [[nodiscard]] bool get_x_flip() const { return get_attribute(x_flip_flag_position); }
        // There are only two palettes, so we can use a single bit
        [[nodiscard]] bool get_palette_number() const { return get_attribute(palette_number_position); }

        [[nodiscard]] int get_y() const { return y; }
        [[nodiscard]] int get_x() const { return x; }
        [[nodiscard]] byte get_tile_number() const { return used_tile; }

        enum size {
            size8x8 = 0,
            size8x16 = 1
        };

        static constexpr byte get_correct_tile_index_for_size(byte index, int y_coordinate,  size size) {
            if (size == size8x8)
                return index;

            bool is_in_second_tile = y_coordinate >= tile::size;
            return utility::write_bit(index, 0, is_in_second_tile);
        }

        static constexpr int width = tile::size;
        static constexpr int get_height_from_size(size size) { return size == size8x8 ? tile::size : tile::size * 2; }

        static constexpr int x_offset = 8;
        static constexpr int y_offset = 16;
    private:

        // data can be private, since the PPU never writes to this
        byte y;
        byte x;
        byte used_tile;
        byte attributes;

        enum {
            priority_flag_position = 7,
            y_flip_flag_position = 6,
            x_flip_flag_position = 5,
            palette_number_position = 4,
        };

        [[nodiscard]] bool get_attribute(int flag_position) const {
            return utility::get_bit(attributes, flag_position);
        }
    };

    // Disposable object meant to be created every scanline
    class sprite_cache {
        static constexpr int max_sprites = 10;
        int sprite_count = 0;

        // Used to speed up the search for sprites at a given x coordinate
        int last_found_sprite_index = 0;

        // Used to skip checking sprites that are left of the current x coordinate, since we presume the x coordinate to
        // be increasing
        int start_index = 0;

        sprite sprites[max_sprites]{};

        // Private so we can control the creation of this object
        sprite_cache() = default;

        void sort_sprites () {
            auto comparator = [](const sprite& a, const sprite& b) {
                return a.get_x() < b.get_x();
            };

            // We need stable sort to preserve the order of sprites with the same x coordinate because they are
            // prioritized by it
            auto begin = std::begin(sprites);
            auto end = begin + sprite_count;

            std::stable_sort(begin, end, comparator);
        }


    public:
        class factory;

        std::optional<sprite> get_first_sprite_at_current_x(int x_coordinate) {
            for (int i = start_index; i < sprite_count; ++i) {
                int offset_x = x_coordinate + sprite::x_offset;

                int sprite_start_x = sprites[i].get_x();
                int sprite_end_x = sprite_start_x + sprite::width;

                // If the x coordinate is lower than the sprite's start x, we can skip the rest of the sprites
                if (offset_x < sprite_start_x) {
                    break;
                }

                start_index = i;
                // If the x coordinate is within the sprite's x range, we have found the first sprite
                if (offset_x >= sprite_start_x && offset_x < sprite_end_x) {
                    last_found_sprite_index = i;
                    return sprites[i];
                }
            }

            return std::nullopt;
        }

        // Only meant to be run after get_first_sprite_at_current_x
        std::optional<sprite> get_next_sprite_at_current_x(int x_coordinate) {
            if (last_found_sprite_index == sprite_count - 1)
                return std::nullopt;

            // We offset the input X so that partially offscreen sprites are still drawn
            int offset_x = x_coordinate + sprite::x_offset;

            int sprite_start_x = sprites[last_found_sprite_index + 1].get_x();
            int sprite_end_x = sprite_start_x + sprite::width;

            if (offset_x >= sprite_start_x && offset_x < sprite_end_x) {
                ++last_found_sprite_index;
                return sprites[last_found_sprite_index];
            }

            return std::nullopt;
        }
    };

    class sprite_cache::factory {
        sprite_cache cache{};

        void finalize() {
            cache.sort_sprites();
        }
    public:
        void add_sprite(sprite sprite) {
            if (cache.sprite_count == max_sprites)
                return;

            cache.sprites[cache.sprite_count++] = sprite;
        }

        sprite_cache create_cache() {
            finalize();
            return cache;
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
        palette sprite_palette_0;
        palette sprite_palette_1;
        byte window_y;
        byte window_x;

        [[nodiscard]] bool get_lcd_display_enable() const { return get_lcd_control_flag(lcd_display_enable); }
        [[nodiscard]] bool get_window_tile_map_select() const { return get_lcd_control_flag(window_tile_map_select); }
        [[nodiscard]] bool get_window_draw_enable() const { return get_lcd_control_flag(window_draw_enable); }
        [[nodiscard]] bool get_bg_and_window_tile_data_select() const { return get_lcd_control_flag(bg_and_window_tile_data_select); }
        [[nodiscard]] bool get_bg_tile_map_select() const { return get_lcd_control_flag(bg_tile_map_select); }
        [[nodiscard]] bool get_sprite_draw_enable() const { return get_lcd_control_flag(sprite_draw_enable); }
        [[nodiscard]] bool get_bg_window_display_priority() const { return get_lcd_control_flag(bg_window_display_priority); }
        [[nodiscard]] sprite::size get_sprite_size() const {
            return get_lcd_control_flag(sprite_size) ? sprite::size::size8x16 : sprite::size::size8x8;
        }

        [[nodiscard]] bool get_coincidence_interrupt_enable() const { return get_lcd_status_flag(coincidence_interrupt_enable); }
        [[nodiscard]] bool get_oam_interrupt_enable() const { return get_lcd_status_flag(oam_interrupt_enable); }
        [[nodiscard]] bool get_v_blank_interrupt_enable() const { return get_lcd_status_flag(v_blank_interrupt_enable); }
        [[nodiscard]] bool get_h_blank_interrupt_enable() const { return get_lcd_status_flag(h_blank_interrupt_enable); }
        [[nodiscard]] bool get_coincidence_flag() const { return get_lcd_status_flag(coincidence_flag); }

        void write_coincidence_flag(bool value) { write_lcd_status_flag(coincidence_flag, value); }

        // Only the lowest two bits correspond to the current mode
        void write_status_mode(byte mode) {
            lcd_status = (lcd_status & 0xFC) | (mode & 0x03);
        }

    private:
        enum lcd_control_flag {
            lcd_display_enable = 7,
            window_tile_map_select = 6,
            window_draw_enable = 5,
            bg_and_window_tile_data_select = 4,
            bg_tile_map_select = 3,
            sprite_size = 2,
            sprite_draw_enable = 1,
            bg_window_display_priority = 0
        };

        [[nodiscard]] bool get_lcd_control_flag(lcd_control_flag flag) const {
            return utility::get_bit(lcd_control, flag);
        }

        enum lcd_status_flags {
            coincidence_interrupt_enable = 6,
            oam_interrupt_enable = 5,
            v_blank_interrupt_enable = 4,
            h_blank_interrupt_enable = 3,
            coincidence_flag = 2,
        };

        [[nodiscard]] bool get_lcd_status_flag(lcd_status_flags flag) const {
            return utility::get_bit(lcd_status, flag);
        }

        void write_lcd_status_flag(lcd_status_flags flag, bool value) const {
            utility::write_bit(lcd_status, flag, value);
        }
    };

    union vram_view {
        static constexpr int size = 0x2000;
    public:
        struct {
            tile_data tiles;
            tile_data::map tile_map_0;
            tile_data::map tile_map_1;
        } tiles;
        byte raw_data[size]{};
    };

    union oam_view {
    private:
        static constexpr int sprite_count = 40;
        static constexpr int size = sprite_count * sizeof(sprite);

    public:
        sprite sprites[sprite_count]{};
        byte raw_data[size];

        sprite_cache create_sprite_cache_for_line(int line_y_coordinate, sprite::size sprite_size) {
            int sprite_height = sprite::get_height_from_size(sprite_size);
            sprite_cache::factory cache_factory{};

            int offset_y = line_y_coordinate + sprite::y_offset;

            for (auto current_sprite : sprites) {
                int sprite_start_y = current_sprite.get_y();
                int sprite_end_y = sprite_start_y + sprite_height;

                if (offset_y >= sprite_start_y && offset_y < sprite_end_y) {
                    cache_factory.add_sprite(current_sprite);
                }
            }

            return cache_factory.create_cache();
        }
    };
}
#endif //SEMESTER_PROJECT_PPU_DATA_HPP
