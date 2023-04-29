
#include <string_view>
#include <iostream>
#include <thread>
#include <SDL.h>

#include "emulator.hpp"

using namespace std::literals::string_view_literals;

constexpr int screen_size_factor = 4;

int main() {
    int window_height = pixel_processing_unit::screen_pixel_height * screen_size_factor;
    int window_width = pixel_processing_unit::screen_pixel_width * screen_size_factor;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* main_window = SDL_CreateWindow("Game Boy Emulator",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              window_width,
                                              window_height,
                                              SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);

    std::string_view boot_rom_path = "/home/adrian/Downloads/dmg_boot.bin"sv;
    std::string_view rom_path = "/home/adrian/Downloads/dr_mario.gb"sv;
    //std::string_view rom_path = "/home/adrian/Downloads/tetris.gb"sv;
    std::string_view sram_path = ""sv;

    emulator::emulator emu(renderer, boot_rom_path, rom_path, sram_path);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while(true) {
        try {
            emu.execute_cpu();
        }
        catch (const emulator::exit&) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(main_window);
            SDL_Quit();

            return 0;
        }
    }
}