// File: ram.hpp
//
// Created by Adrian Habusta on 27.04.2023
//

#ifndef SEMESTER_PROJECT_RAM_HPP
#define SEMESTER_PROJECT_RAM_HPP

#include "../utility.hpp"

namespace random_access_memory {
    struct ram {
        byte read_wram(word address) { return wram[address]; }
        void write_wram(word address, byte value) { wram[address] = value;}

        byte read_hram(word address) { return hram[address]; }
        void write_hram(word address, byte value) { hram[address] = value; }

    private:
        static constexpr int wram_size = 0x2000;
        static constexpr int hram_size = 0x7F;

        byte wram[wram_size];
        byte hram[hram_size];
    };
}

#endif //SEMESTER_PROJECT_RAM_HPP
