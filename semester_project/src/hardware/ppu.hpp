// File: ppu.hpp
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

namespace ppu {
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
        static constexpr int t_cycles_per_hblank = 204;
        static constexpr int t_cycles_per_vblank = 4560;
        static constexpr int t_cycles_per_oam_read = 80;
        static constexpr int t_cycles_per_pixel_transfer = 172;

        static constexpr int t_cycles_per_scanline = t_cycles_per_hblank + t_cycles_per_pixel_transfer + t_cycles_per_oam_read;

        static constexpr int scanlines_per_frame = 154;
        static constexpr int scanlines_per_screen = 144;
        static constexpr int scanlines_per_vblank = 10;

        enum mode {
            h_blank = 0,
            v_blank = 1,
            oam_search = 2,
            pixel_transfer = 3
        };

        static constexpr int mode_length[] = {
            t_cycles_per_hblank,
            t_cycles_per_vblank,
            t_cycles_per_oam_read,
            t_cycles_per_pixel_transfer
        };

        bool is_powered_on{true};
        mode current_mode{};
        int remaining_t_cycles{};

        std::optional<sprite_cache> current_line_sprites;

        interrupt_callback request_cpu_interrupt;

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
        palette::pixel get_pixel_from_sprite(int x, sprite current_sprite);
        pixel get_pixel_from_background_layer(int x) const;
        pixel get_pixel_from_window_layer(int x) const;
        bool check_if_in_window(int x) const;

        void move_to_next_mode();
        void change_mode_to(mode new_mode);
        void request_mode_change_interrupt(mode new_mode);

        void increment_line_counter_and_check_for_match();

        void power_on();
        void power_off();
    public:
        ppu(SDL_Renderer *renderer, interrupt_callback callback) : request_cpu_interrupt(callback),
                                                                   renderer (renderer) {}

        void run_m_cycle();
    };
}

#endif //SEMESTER_PROJECT_PPU_HPP
