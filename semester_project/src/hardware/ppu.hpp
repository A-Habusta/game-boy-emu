// File: ppu.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_PPU_HPP
#define SEMESTER_PROJECT_PPU_HPP

#include <functional>
#include <cstring>
#include <SDL.h>

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
        ppu_renderer renderer;
        register_file registers;

        vram_struct vram;
        oam_struct oam;
    public:
        ppu(SDL_Renderer *renderer) : renderer(renderer) {}
    };
}

#endif //SEMESTER_PROJECT_PPU_HPP
