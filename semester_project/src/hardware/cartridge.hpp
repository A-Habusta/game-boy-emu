// File: cartridge.hpp
//
// Created by Adrian Habusta on 26.04.2023
//

#ifndef SEMESTER_PROJECT_CARTRIDGE_HPP
#define SEMESTER_PROJECT_CARTRIDGE_HPP

#include <string>
#include "../utility.hpp"

// TODO Implement
class cartridge {
    void load_from_file(const std::string& path) {
        return;
    }

    byte read_rom(word address) {
        return 0;
    }

    byte read_ram(word address) {
        return 0;
    }

    // This is used for controlling the MBC
    void write_rom(word address, byte value) {
        return;
    }

    void write_ram(word address, byte value) {
        return;
    }
};

#endif //SEMESTER_PROJECT_CARTRIDGE_HPP
