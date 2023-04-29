// File: cartridge_memory_controllers.cpp
//
// Created by Adrian Habusta on 28.04.2023
//

#include <iostream>
#include <fstream>
#include <string>

#include "cartridge_memory_controllers.hpp"

void plain_rom::load_rom_from_file(std::string_view path) {
    return utility::read_file(path, rom, rom_size, "Failed to load ROM");
}