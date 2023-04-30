
#include <string_view>
#include <iostream>
#include <thread>
#include <SDL.h>

#include "emulator.hpp"

constexpr int screen_size_factor = 4;

void free_sdl(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "Not enough arguments! Expected 2 or 3." << std::endl;
        return 1;
    }

    if(argc > 4) {
        std::cout << "Too many arguments! Expected 2 or 3." << std::endl;
        return 1;
    }

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

    std::string_view boot_rom_path = argv[1];
    std::string_view rom_path = argv[2];
    std::string_view sram_path = argc == 4 ? argv[3] : "";

    std::optional<emulator::emulator> emu{std::nullopt};
    try {
        emu.emplace(renderer, boot_rom_path, rom_path, sram_path);
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        free_sdl(renderer, main_window);
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while(true) {
        try {
            emu->execute_cpu();
        }
        catch (const emulator::exit&) {
            free_sdl(renderer, main_window);
            return 0;
        }
    }
}