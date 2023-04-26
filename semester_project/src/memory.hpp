// File: memory.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_MEMORY_HPP
#define SEMESTER_PROJECT_MEMORY_HPP

#include "utility.hpp"

class cpu;
class ppu;
class apu;
class timer;
class cartridge;
class random_access_memory;


namespace memory {
    struct random_access_memory {
        byte read_wram(word address);
        void write_wram(word address, byte value);

        byte read_hram(word address);
        void write_hram(word address, byte value);

    private:
        static constexpr int wram_size = 0x2000;
        static constexpr int hram_size = 0x7F;

        byte wram[wram_size];
        byte hram[hram_size];
    };

    class memory_map {
        cpu& cpu_memory;
        pixel_processing_unit& ppu_memory;
        apu& apu_memory;
        timer& timer_memory;
        cartridge& cartridge_memory;
        random_access_memory ram;

    public:
        memory_map(cpu& cpu, pixel_processing_unit& ppu, apu& apu, timer& timer, cartridge& cartridge) :
                cpu_memory(cpu), ppu_memory(ppu), apu_memory(apu), timer_memory(timer), cartridge_memory(cartridge) {};
        byte read_with_cycling(word address);
        void write_with_cycling(word address, byte value);
    };
}

#endif //SEMESTER_PROJECT_MEMORY_HPP
