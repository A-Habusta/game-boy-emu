#include <SDL.h>
#include <iostream>

#include "emulator.hpp"

constexpr int screen_size_factor = 2;

int main() {
    int window_height = pixel_processing_unit::screen_pixel_height * screen_size_factor;
    int window_width = pixel_processing_unit::screen_pixel_width * screen_size_factor;

    /*
    SDL_Window* main_window = SDL_CreateWindow("Gameboy Emulator",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              window_width,
                                              window_height,
                                              SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    */
    SDL_Renderer* renderer = nullptr;

    emulator::emulator emu(renderer);

    word addr = 0xFF30;
    word addr_ram = 0xC000;
    emu.write_with_cycling(addr_ram, 'a');
    std::cout << emu.read_with_cycling(addr) << std::endl;
    std::cout << emu.read_with_cycling(addr_ram) << std::endl;
    return 0;
}