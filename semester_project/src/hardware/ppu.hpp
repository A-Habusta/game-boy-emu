// File: pixel_processing_unit.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_PPU_HPP
#define SEMESTER_PROJECT_PPU_HPP

#include <functional>
#include <optional>
#include <cstring>
#include <SDL.h>

#include "../cpu/cpu_interrupt_typedef.hpp"
#include "ppu_data.hpp"

namespace pixel_processing_unit {
    constexpr int screen_pixel_width = 160;
    constexpr int screen_pixel_height = 144;

    class ppu_renderer {
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        palette::real_pixel_type screen_buffer[screen_pixel_height][screen_pixel_width];

        void push_buffer_to_texture() {
            palette::real_pixel_type *pixels;
            int discard;

            SDL_LockTexture(texture, nullptr, (void**)(&pixels), &discard);
            memcpy(pixels, screen_buffer, sizeof(screen_buffer));
            SDL_UnlockTexture(texture);
        }
    public:
        ppu_renderer(SDL_Renderer *renderer) : renderer(renderer) {
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                        screen_pixel_width, screen_pixel_height);
        }

        void save_pixel(int x, int y, palette::real_pixel_type color) {
            screen_buffer[y][x] = color;
        }

        void render_frame() {
            SDL_RenderClear(renderer);

            push_buffer_to_texture();
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);

            SDL_RenderPresent(renderer);
        }
        void render_blank_frame() {
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    };

    class ppu {
        static constexpr int t_cycles_per_m_cycle = 4;
        // Actual h_blank and pixel_transfer modes can last a variable amount of time depending on how many sprites are
        // on the current scanline and other factors. We will not simulate this, so we will just use the maximum length
        // hlbank.
        static constexpr int t_cycles_per_h_blank = 204;
        static constexpr int t_cycles_per_v_blank = 4560;
        static constexpr int t_cycles_per_oam_read = 80;
        static constexpr int t_cycles_per_pixel_transfer = 172;

        static constexpr int t_cycles_per_scanline = t_cycles_per_h_blank + t_cycles_per_pixel_transfer + t_cycles_per_oam_read;

        static constexpr int scanlines_per_frame = 154;
        static constexpr int scanlines_per_screen = 144;
        static constexpr int scanlines_per_v_blank = 10;

        enum mode {
            h_blank = 0,
            v_blank = 1,
            oam_search = 2,
            pixel_transfer = 3
        };

        static constexpr int mode_length[] = {
                t_cycles_per_h_blank,
                t_cycles_per_v_blank,
                t_cycles_per_oam_read,
                t_cycles_per_pixel_transfer
        };

        bool is_powered_on{true};
        mode current_mode{};
        int remaining_t_cycles{};

        std::optional<sprite_cache> current_line_sprites;

        interrupt_callback request_stat_interrupt;
        interrupt_callback request_v_blank_interrupt;

        ppu_renderer renderer;
        register_file registers{};

        vram_view vram{};
        oam_view oam{};

        bool is_vram_blocked() const { return current_mode == mode::pixel_transfer; }
        bool is_oam_blocked() const { return current_mode == mode::pixel_transfer || current_mode == mode::oam_search; }

        void run_t_cycle();

        void run_h_blank_t_cycle();
        void run_v_blank_t_cycle();
        void run_oam_search_t_cycle();
        void run_pixel_transfer_t_cycle();

        // y is implicit;
        palette::pixel get_pixel_from_sprite(int x, sprite current_sprite, sprite::size current_size);
        palette::pixel get_pixel_from_background_layer(int x) const;
        palette::pixel get_pixel_from_window_layer(int x) const;
        bool check_if_in_window(int x) const;

        void move_to_next_mode();
        void change_mode_to(mode new_mode);
        void request_mode_change_interrupt(mode new_mode);

        void increment_line_counter_and_check_for_match();

        void power_on() {
            is_powered_on = true;
            registers.lcd_y = 0;
            current_mode = mode::h_blank;
            remaining_t_cycles = mode_length[current_mode];
        }

        void power_off() {
            is_powered_on = false;
            renderer.render_blank_frame();
        }

    public:
        ppu(SDL_Renderer *renderer, interrupt_callback stat_callback, interrupt_callback v_blank_callback)
            : request_stat_interrupt(stat_callback), request_v_blank_interrupt(v_blank_callback), renderer (renderer) {}

        void run_machine_cycle();

        byte read_vram(word address) {
            if (is_vram_blocked())
                return 0xFF;

            return vram.raw_data[address];
        }
        byte read_oam(word address) {
            if (is_oam_blocked())
                return 0xFF;

            return oam.raw_data[address];
        }

        void write_vram(word address, byte value) {
            if (is_vram_blocked())
                return;

            vram.raw_data[address] = value;
        }
        void write_oam(word address, byte value) {
            if (is_oam_blocked())
                return;

            oam.raw_data[address] = value;
        }

        // Basically a raw write
        void write_oam_dma(word address, byte value) {
            oam.raw_data[address] = value;
        }

        byte read_lcd_control() const { return registers.lcd_control; }
        byte read_lcd_status() const { return registers.lcd_status; }
        byte read_scroll_y() const { return registers.scroll_y; }
        byte read_scroll_x() const { return registers.scroll_x; }
        byte read_lcd_y() const { return registers.lcd_y; }
        byte read_lcd_y_compare() const { return registers.lcd_y_compare; }
        byte read_dma_transfer() const { return registers.dma_transfer; }
        byte read_bg_palette() const { return registers.background_palette.read_raw_value(); }
        byte read_sprite_palette_0() const { return registers.sprite_palette_0.read_raw_value(); }
        byte read_sprite_palette_1() const { return registers.sprite_palette_1.read_raw_value(); }
        byte read_window_y() const { return registers.window_y; }
        byte read_window_x() const { return registers.window_x; }

        void write_lcd_status(byte value) { registers.lcd_status = value; }
        void write_scroll_y(byte value) { registers.scroll_y = value; }
        void write_scroll_x(byte value) { registers.scroll_x = value; }
        void write_lcd_y(byte value) { registers.lcd_y = value; }
        void write_lcd_y_compare(byte value) { registers.lcd_y_compare = value; }
        void write_dma_transfer(byte value) { registers.dma_transfer = value; }
        void write_bg_palette(byte value) { registers.background_palette.write_raw_value(value); }
        void write_sprite_palette_0(byte value) { registers.sprite_palette_0.write_raw_value(value); }
        void write_sprite_palette_1(byte value) { registers.sprite_palette_1.write_raw_value(value); }
        void write_window_y(byte value) { registers.window_y = value; }
        void write_window_x(byte value) { registers.window_x = value; }

        void write_lcd_control(byte value) {
            bool requested_power_status = utility::get_bit(value, 7);

            if (requested_power_status && !is_powered_on)
                power_on();

            else if (!requested_power_status && is_powered_on)
                power_off();

            registers.lcd_control = value;
        }
    };
}

#endif //SEMESTER_PROJECT_PPU_HPP
