// File: pixel_processing_unit.hpp
//
// Created by Adrian Habusta on 22.04.2023
//

#ifndef SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP
#define SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP

#include <functional>
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

    constexpr int OAM_SPRITE_COUNT = 40;
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
    constexpr int TIMING_MODE_0_MAX_CYCLES = TIMING_SCANLINE_CYCLES - TIMING_MODE_1_CYCLES - TIMING_MODE_2_CYCLES -
            TIMING_MODE_3_MIN_CYCLES;

    constexpr int MAX_SCANLINE = 153;
    constexpr int LAST_DRAWN_SCANLINE = SCREEN_HEIGHT - 1;


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

        enum size {
            size8x8 = 64,
            size8x16 = 128
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

    // Right now in ARGB8888 format, which is a bit wasteful, since there are only 4 possible colors
    struct pixel {
        pixel() = default;
        pixel(uint32_t color) : color(color) {}

        uint32_t color;
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

        //LYC == LC
        bool get_coincidence_flag() { return utility::get_bit(lcd_status, LCD_STATUS_COINCIDENCE_FLAG_POSITION); }
        void set_coincidence_flag(bool value) { utility::write_bit(lcd_status, LCD_STATUS_COINCIDENCE_FLAG_POSITION,
                                                                   value); }

        bool get_coincidence_interrupt_flag() { return utility::get_bit(lcd_status, LCD_STATUS_LYC_INTERRUPT_FLAG_POSITION); }
        bool get_mode_2_oam_interrupt_flag() { return utility::get_bit(lcd_status, LCD_STATUS_MODE_2_OAM_INTERRUPT_FLAG_POSITION); }
        bool get_mode_1_vblank_interrupt_flag() { return utility::get_bit(lcd_status, LCD_STATUS_MODE_1_VBLANK_INTERRUPT_FLAG_POSITION); }
        bool get_mode_0_hblank_interrupt_flag() { return utility::get_bit(lcd_status, LCD_STATUS_MODE_0_HBLANK_INTERRUPT_FLAG_POSITION); }

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

            LCD_STATUS_LYC_INTERRUPT_FLAG_POSITION = 6,
            LCD_STATUS_MODE_2_OAM_INTERRUPT_FLAG_POSITION = 5,
            LCD_STATUS_MODE_1_VBLANK_INTERRUPT_FLAG_POSITION = 4,
            LCD_STATUS_MODE_0_HBLANK_INTERRUPT_FLAG_POSITION = 3,
            LCD_STATUS_COINCIDENCE_FLAG_POSITION = 2,
        };
    };


    class ppu {
        ppu_register_file registers{};

        union oam {
            sprite sprites[OAM_SPRITE_COUNT]{};
            byte bytes[OAM_SPRITE_COUNT * sizeof(sprite)];
        } oam{};

        int sprite_buffer_relevant_index = 0;
        sprite sprite_buffer[MAX_SPRITES_PER_LINE]{};

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


        inline static pixel master_palette[palette::COLOR_COUNT]{ 0xFF000000, 0xFF555555, 0xFFAAAAAA, 0xFFFFFFFF };

        bool power = true;
        int t_cycles = 0;

        enum mode {
            OAM_SEARCH = 2,
            PIXEL_TRANSFER = 3,
            H_BLANK = 0,
            V_BLANK = 1
        } current_mode = mode::H_BLANK;

        interrupt_callback request_interrupt;


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


        void set_mode_flag_to_current_mode() {
            byte mode_flag = (byte) current_mode;
            registers.lcd_status &= 0xFC;
            registers.lcd_status |= mode_flag;
        }

        // Inaccurate. The interrupt logic is a bit finicky and not supporting it perfectly affects only a few games.
        void request_mode_change_interrupt(mode new_mode);

        void change_mode(mode new_mode) {
            current_mode = new_mode;
            request_mode_change_interrupt(new_mode);

            set_mode_flag_to_current_mode();
            t_cycles = 0;
        }

        void power_off() {
            power = false;
            reset_lcd_y();

            render_blank_screen();
        }

        void power_on() {
            power = true;
            change_mode(mode::H_BLANK);
        }

        void increment_lcd_y() {
            registers.lcd_y = (registers.lcd_y + 1) % (MAX_SCANLINE + 1);

            bool compare = registers.lcd_y == registers.lcd_y_compare;
            registers.set_coincidence_flag(compare);

            if (compare && registers.get_coincidence_interrupt_flag()) {
                request_interrupt();
            }
        }

        void reset_lcd_y() {
            registers.lcd_y = 0;
        }

        void run_t_cycle();
        void run_oam_search_t_cycle();
        void run_pixel_transfer_t_cycle();
        void run_h_blank_t_cycle();
        void run_v_blank_t_cycle();

        void populate_sprite_buffer_for_current_line();
        // Rendering to screen buffer
        void render_pixel_to_buffer(int relative_pixel_x, int relative_pixel_y);

        pixel get_sprite_layer_pixel(int relative_pixel_x, int relative_pixel_y);
        pixel get_window_layer_pixel(int relative_pixel_x, int relative_pixel_y);
        pixel get_background_layer_pixel(int absolute_pixel_x, int absolute_pixel_y);

        void push_pixel_to_screen_buffer(int x, int y, pixel pixel) {
            int index = y * SCREEN_WIDTH + x;
            screen_buffer[index] = pixel;
        }

        // Rendering to actual screen
        void flush_buffer_to_texture();
        void render_screen_buffer();

        // NOT accurate, the original game boy is supposed to have a black line in the middle of the screen
        void render_blank_screen();

    public:
        ppu(SDL_Renderer* renderer, interrupt_callback interrupt);
        void run_m_cycle();
        // Memory map instructions
        byte read_vram_generic(word address);
        void write_vram_generic(word address, byte value);

        byte read_oam_generic(word address);
        void write_oam_generic(word address, byte value);
    };
}

#endif //SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP
