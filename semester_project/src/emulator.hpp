// File: emulator.hpp
//
// Created by Adrian Habusta on 22.04.2023
//

#ifndef SEMESTER_PROJECT_EMULATOR_HPP
#define SEMESTER_PROJECT_EMULATOR_HPP

#include "cpu/central_processing_unit.hpp"
#include "hardware/timer.hpp"
#include "hardware/ppu.hpp"
#include "hardware/apu.hpp"
#include "memory.hpp"
#include "utility.hpp"

class emulator {
    central_processing_unit::cpu cpu;
    memory::memory_map memory;
    timer timer;
    pixel_processing_unit::ppu ppu;
    apu::apu apu;

    bool running = true;
private:

public:
    emulator();

    class stop{};
    void run_cycle();
};

#endif //SEMESTER_PROJECT_EMULATOR_HPP
