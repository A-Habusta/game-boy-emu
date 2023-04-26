// File: emulator.cpp
//
// Created by Adrian Habusta on 22.04.2023
//

#include "emulator.hpp"

emulator::emulator() : memory(), cpu() {
    SDL_Window* window = SDL_CreateWindow("Game Boy Emu",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          pixel_processing_unit::screen_pixel_width * 2,
                                          pixel_processing_unit::screen_pixel_height * 2,
                                          0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    cpu.set_phantom_cycle_callback([this](){ run_cycle(); });

    timer = timer::timer( std::bind(cpu.request_lcd_stat_interrupt()) );


}

void emulator::run_cycle() {
    ppu.run_m_cycle();
    apu.run_cycle();
    timer.run_m_cycle();
}