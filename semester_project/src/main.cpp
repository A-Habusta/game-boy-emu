#include <SDL.h>
#include <iostream>

#include "emulator.hpp"

constexpr int screen_size_factor = 2;

int main() {
    int window_height = pixel_processing_unit::screen_pixel_height * screen_size_factor;
    int window_width = pixel_processing_unit::screen_pixel_width * screen_size_factor;

    SDL_Window* main_window = SDL_CreateWindow("Gameboy Emulator",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              window_width,
                                              window_height,
                                              SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    emulator::emulator emu(renderer);

    emu.reset_cpu();
    while(true) {
        SDL_RenderPresent(renderer);
        emu.execute_cpu();
    }
}