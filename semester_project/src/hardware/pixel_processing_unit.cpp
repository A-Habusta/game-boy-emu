// File: pixel_processing_unit.cpp
//
// Created by Adrian Habusta on 22.04.2023
//

#include "pixel_processing_unit.hpp"

namespace pixel_processing_unit {
    ppu::ppu(SDL_Renderer *renderer) : renderer(renderer) {
        screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
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
        for (int i = 0; i < 4; i++)
            run_t_cycle();
    }

    void ppu::run_t_cycle() {

    }

    void ppu::push_pixel_to_screen_buffer(int x, int y, pixel color) {
        int index = y * SCREEN_WIDTH + x;
        screen_buffer[index] = color;
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

}