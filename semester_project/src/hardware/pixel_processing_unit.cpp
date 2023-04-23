// File: pixel_processing_unit.cpp
//
// Created by Adrian Habusta on 22.04.2023
//

#include <algorithm>

#include "pixel_processing_unit.hpp"

namespace pixel_processing_unit {
    ppu::ppu(SDL_Renderer *renderer, interrupt_callback interrupt) : renderer(renderer), request_interrupt(interrupt) {
        screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                           SCREEN_HEIGHT);
    }

    byte ppu::read_vram_generic(word address) {
        if(is_vram_blocked())
            return utility::UNDEFINED_BYTE;

        return vram.bytes[address];
    }
    void ppu::write_vram_generic(word address, byte value) {
        if(is_vram_blocked())
            return;

        vram.bytes[address] = value;
    }

    byte ppu::read_oam_generic(word address) {
        if (is_oam_blocked())
            return utility::UNDEFINED_BYTE;

        return oam.bytes[address];
    }
    void ppu::write_oam_generic(word address, byte value) {
        if (is_oam_blocked())
            return;

        oam.bytes[address] = value;
    }

    void ppu::run_m_cycle() {
        if (!power)
            return;

        for (int i = 0; i < 4; i++)
            run_t_cycle();
    }

    void ppu::request_mode_change_interrupt(mode new_mode) {
        bool interrupt_requested = false;

        switch (new_mode) {
            case mode::H_BLANK:
                interrupt_requested |= registers.get_mode_0_hblank_interrupt_flag();
                break;
            case mode::V_BLANK:
                interrupt_requested |= registers.get_mode_1_vblank_interrupt_flag();
                // NO BREAK IS INTENTIONAL, OAM interrupt is also requested in V_BLANK
            case mode::OAM_SEARCH:
                interrupt_requested |= registers.get_mode_2_oam_interrupt_flag();
                break;

            default: break;
        }

        if (interrupt_requested)
            request_interrupt();
    }


    void ppu::run_t_cycle() {
        switch (current_mode) {
            case mode::OAM_SEARCH: run_oam_search_t_cycle(); break;
            case mode::PIXEL_TRANSFER: run_pixel_transfer_t_cycle(); break;
            case mode::H_BLANK: run_h_blank_t_cycle(); break;
            case mode::V_BLANK: run_v_blank_t_cycle(); break;

            default: break;
        }
    }

    void ppu::run_oam_search_t_cycle() {
        if (t_cycles++ != 0) {
            if (t_cycles == TIMING_MODE_2_CYCLES) {
                change_mode(mode::PIXEL_TRANSFER);
            }
            return;
        }

        populate_sprite_buffer_for_current_line();
    }

    void ppu::run_pixel_transfer_t_cycle() {
        if (t_cycles++ >= SCREEN_WIDTH) {
            if (t_cycles == TIMING_MODE_3_MIN_CYCLES) {
                change_mode(mode::H_BLANK);
            }
            return;
        }

        int pixel_x = registers.lcd_y;
        int pixel_y = t_cycles - 1;

        render_pixel_to_buffer(pixel_x, pixel_y);
    }

    void ppu::run_h_blank_t_cycle() {
        if (t_cycles++ != 0) {
            if (t_cycles == TIMING_MODE_0_MAX_CYCLES) {
                if (registers.lcd_y == LAST_DRAWN_SCANLINE) {
                    change_mode(mode::V_BLANK);
                } else {
                    change_mode(mode::OAM_SEARCH);
                }
            }
            return;
        }
    }

        void ppu::run_v_blank_t_cycle() {
        if (t_cycles++ != 0) {
            if (t_cycles == TIMING_MODE_1_SCANLINES * TIMING_SCANLINE_CYCLES) {
                change_mode(mode::OAM_SEARCH);
            } else if (t_cycles % TIMING_SCANLINE_CYCLES == 0) {
                increment_lcd_y();
            }
        }

        flush_buffer_to_texture();
        render_screen_buffer();
    }


    void ppu::populate_sprite_buffer_for_current_line() {
        int current_cached_sprites = 0;
        int sprite_height = registers.get_obj_size() / TILE_WIDTH;

        for (int i = 0; i < OAM_SPRITE_COUNT && current_cached_sprites < MAX_SPRITES_PER_LINE; i++) {
            sprite sprite = oam.sprites[i];
            int sprite_start_y = sprite.y;
            int sprite_end_y = sprite_start_y + sprite_height;

            if (registers.lcd_y >= sprite_start_y && registers.lcd_y < sprite_end_y) {
                cached_sprites[current_cached_sprites++] = sprite;
            }
        }

        auto compare_sprites = [](const sprite a, const sprite b) { return a.x < b.x; };

        std::stable_sort(std::begin(cached_sprites), std::end(cached_sprites), compare_sprites);

        sprite_buffer_relevant_index = 0;
    }


    void ppu::render_pixel_to_buffer(int relative_pixel_x, int relative_pixel_y) {
        pixel pixel;

        //TODO ??? how

        push_pixel_to_screen_buffer(relative_pixel_x, relative_pixel_y, pixel);
    }


    void ppu::flush_buffer_to_texture() {
        pixel* pixels;
        int pitch;

        SDL_LockTexture(screen_texture, nullptr, (void**)&pixels, &pitch);

        memcpy(pixels, screen_buffer, SCREEN_SIZE * sizeof(pixel));

        SDL_UnlockTexture(screen_texture);
    }

    void ppu::render_screen_buffer() {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screen_texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    void ppu::render_blank_screen() {
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
};