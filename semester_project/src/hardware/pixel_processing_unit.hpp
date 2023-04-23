// File: pixel_processing_unit.hpp
//
// Created by Adrian Habusta on 22.04.2023
//

#ifndef SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP
#define SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP

#include <SDL.h>
#include "../utility.hpp"

namespace pixel_processing_unit {
    constexpr int SCREEN_WIDTH = 160;
    constexpr int SCREEN_HEIGHT = 144;
    constexpr int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

    constexpr int TILE_WIDTH = 8;
    constexpr int TILE_HEIGHT = 8;
    constexpr int TILE_BYTES_PER_LINE = 2;
    constexpr int BYTES_PER_TILE = TILE_HEIGHT * TILE_BYTES_PER_LINE;

    constexpr int SPRITE_COUNT = 40;
    constexpr int MAX_SPRITES_PER_LINE = 10;

    constexpr int VRAM_TILE_AREA_START = 0x0000;
    constexpr int VRAM_TILE_AREA_END = 0x17FF;
    constexpr int VRAM_TILE_AREA_SIZE = VRAM_TILE_AREA_END - VRAM_TILE_AREA_START + 1;
    constexpr int VRAM_TILE_COUNT = VRAM_TILE_AREA_SIZE / BYTES_PER_TILE;

    constexpr int VRAM_TILE_MAP_WIDTH = 32;
    constexpr int VRAM_TILE_MAP_HEIGHT = 32;

    constexpr int VRAM_TILE_MAP_0_START = 0x1800;
    constexpr int VRAM_TILE_MAP_0_END = 0x1BFF;
    constexpr int VRAM_TILE_MAP_0_SIZE = VRAM_TILE_MAP_0_END - VRAM_TILE_MAP_0_START + 1;

    constexpr int VRAM_TILE_MAP_1_START = 0x1C00;
    constexpr int VRAM_TILE_MAP_1_END = 0x1FFF;
    constexpr int VRAM_TILE_MAP_1_SIZE = VRAM_TILE_MAP_1_END - VRAM_TILE_MAP_1_START + 1;

    constexpr int VRAM_SIZE = VRAM_TILE_AREA_SIZE + VRAM_TILE_MAP_0_SIZE + VRAM_TILE_MAP_1_SIZE;

    constexpr int VRAM_METHOD_8800_OFFSET = 0x0800;

    // These cycles are counted on a 4.19 MHz clock
    // We are not going to deal with timing right now, the rendering timing will act as if no sprites/scrolling is
    // happening
    constexpr int TIMING_SCANLINE_CYCLES = 456;
    constexpr int TIMING_MODE_1_SCANLINES = 10;
    constexpr int TIMING_MODE_1_CYCLES = TIMING_MODE_1_SCANLINES * TIMING_SCANLINE_CYCLES;
    constexpr int TIMING_MODE_2_CYCLES = 80;
    constexpr int TIMING_MODE_3_MIN_CYCLES = 172;


    struct tile {
        byte get_pixel_color(int x, int y) const {
            byte color = 0;
            color |= utility::get_bit(data[y * TILE_BYTES_PER_LINE], 7 - x);
            color |= utility::get_bit(data[y * TILE_BYTES_PER_LINE + 1], 7 - x) << 1;
            return color;
        }
    private:
        byte data[BYTES_PER_TILE]{};
    };

    struct sprite {
        byte y{};
        byte x{};
        byte tile{};
        byte attributes{};

        enum class size {
            size8x8,
            size8x16
        };

        bool get_priority_flag() { return utility::get_bit(attributes, PRIORITY_FLAG_POSITION); }
        bool get_y_flip_flag() { return utility::get_bit(attributes, Y_FLIP_FLAG_POSITION); }
        bool get_x_flip_flag() { return utility::get_bit(attributes, X_FLIP_FLAG_POSITION); }
        byte get_palette_number() { return utility::get_bit(attributes, PALETTE_NUMBER_POSITION); }

    private:
        enum {
            PRIORITY_FLAG_POSITION = 7,
            Y_FLIP_FLAG_POSITION = 6,
            X_FLIP_FLAG_POSITION = 5,
            PALETTE_NUMBER_POSITION = 4
        };
    };

    // Right now in RGB24 format, which is a bit wasteful, but oh well
    struct pixel {
        pixel() = default;
        pixel(uint32_t color) : r(color >> 16), g(color >> 8), b(color) {}

        byte r{};
        byte g{};
        byte b{};
    };

    struct palette {
        static constexpr int COLOR_COUNT = 4;

        byte color{};

        byte get_master_color_index(int index) {
            index %= COLOR_COUNT;
            byte result_color = 0;

            result_color |= utility::get_bit(color, index * 2);
            result_color |= utility::get_bit(color, index * 2 + 1) << 1;

            return result_color;
        }
    };

    struct ppu_register_file {
        byte lcd_control{};
        byte lcd_status{};
        byte scroll_y{};
        byte scroll_x{};
        byte lcd_y{};
        byte lcd_y_compare{};
        byte dma_transfer{};
        palette bg_palette{};
        palette obj_palette_0{};
        palette obj_palette_1{};
        byte window_y{};
        byte window_x{};

        bool get_display_enable() { return utility::get_bit(lcd_control, LCD_CONTROL_DISPLAY_ENABLE_POSITION); }
        bool get_window_tile_map_select() { return utility::get_bit(lcd_control, LCD_CONTROL_WINDOW_TILE_MAP_SELECT_POSITION); }
        bool get_window_enable() { return utility::get_bit(lcd_control, LCD_CONTROL_WINDOW_ENABLE_POSITION); }
        bool get_bg_and_window_tile_data_select() { return utility::get_bit(lcd_control, LCD_CONTROL_BG_AND_WINDOW_TILE_DATA_SELECT_POSITION); }
        bool get_bg_tile_map_select() { return utility::get_bit(lcd_control, LCD_CONTROL_BG_TILE_MAP_SELECT_POSITION); }
        sprite::size get_obj_size() {
            return utility::get_bit(lcd_control, LCD_CONTROL_OBJ_SIZE_POSITION) ? sprite::size::size8x16 : sprite::size::size8x8;
        }
        bool get_obj_display_enable() { return utility::get_bit(lcd_control, LCD_CONTROL_OBJ_DISPLAY_ENABLE_POSITION); }
        bool get_bg_display() { return utility::get_bit(lcd_control, LCD_CONTROL_BG_AND_WINDOW_DISPLAY_POSITION); }

    private:
        enum {
            LCD_CONTROL_DISPLAY_ENABLE_POSITION = 7,
            LCD_CONTROL_WINDOW_TILE_MAP_SELECT_POSITION = 6,
            LCD_CONTROL_WINDOW_ENABLE_POSITION = 5,
            LCD_CONTROL_BG_AND_WINDOW_TILE_DATA_SELECT_POSITION = 4,
            LCD_CONTROL_BG_TILE_MAP_SELECT_POSITION = 3,
            LCD_CONTROL_OBJ_SIZE_POSITION = 2,
            LCD_CONTROL_OBJ_DISPLAY_ENABLE_POSITION = 1,
            LCD_CONTROL_BG_AND_WINDOW_DISPLAY_POSITION = 0,
        };
    };


    class ppu {
        ppu_register_file registers{};

        union oam {
            sprite sprites[SPRITE_COUNT]{};
            byte bytes[SPRITE_COUNT * sizeof(sprite)];
        } oam{};

        union vram {
            struct tile_view {
                tile tiles[VRAM_TILE_COUNT];
                byte tile_map_0[VRAM_TILE_MAP_0_SIZE];
                byte tile_map_1[VRAM_TILE_MAP_1_SIZE];

                tile get_tile_method_8000(byte offset) {
                    return tiles[offset];
                };

                tile get_tile_method_8800(byte offset) {
                    int8_t signed_offset = (int8_t) (offset);
                    return tiles[VRAM_METHOD_8800_OFFSET + signed_offset];
                };

                byte get_tile_map_entry(int x, int y, bool is_tile_map_1) {
                    int tile_map_index = y * VRAM_TILE_MAP_WIDTH + x;
                    return is_tile_map_1 ? tile_map_1[tile_map_index] : tile_map_0[tile_map_index];
                }
            } tile_view{};
            byte bytes[VRAM_SIZE];
        } vram{};

        SDL_Renderer* renderer;
        SDL_Texture* screen_texture;
        pixel screen_buffer[SCREEN_SIZE]{};

        pixel master_palette[palette::COLOR_COUNT]{ 0x000000, 0x555555, 0xAAAAAA, 0xFFFFFF };

        int t_cycles = 0;

        enum mode {
            OFF = 0xFF,
            OAM_SEARCH = 2,
            PIXEL_TRANSFER = 3,
            H_BLANK = 0,
            V_BLANK = 1
        } current_mode = mode::OFF;

        tile get_tile_from_oam_index(byte offset) {
            return vram.tile_view.get_tile_method_8000(offset);
        }

        tile get_tile_from_bg_or_window_index(byte offset) {
            bool method = registers.get_bg_and_window_tile_data_select();
            return method ? vram.tile_view.get_tile_method_8800(offset) : vram.tile_view.get_tile_method_8000(offset);
        }

        bool is_vram_blocked() {
            return current_mode == mode::PIXEL_TRANSFER;
        }

        bool is_oam_blocked() {
            return current_mode == mode::PIXEL_TRANSFER || current_mode == mode::OAM_SEARCH;
        }

        pixel get_pixel_color_from_palette(palette palette, byte color_index) {
            byte master_color_index = palette.get_master_color_index(color_index);
            return master_palette[master_color_index];
        }

        void run_t_cycle();
        void push_pixel_to_screen_buffer(int x, int y, pixel pixel_color);

        void flush_buffer_to_texture();
        void render_screen_buffer();
    public:
        ppu(SDL_Renderer* renderer);
        void run_m_cycle();
        // Memory map instructions
        byte read_vram_generic(word address);
        void write_vram_generic(word address, byte value);

        byte read_oam_generic(word address);
        void write_oam_generic(word address, byte value);
    };
}

#endif //SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP
