// File: cartridge.cpp
//
// Created by Adrian Habusta on 26.04.2023
//

#include "cartridge.hpp"

#include <fstream>
#include "cartridge_memory_controllers.hpp"

cartridge::cartridge(std::string_view boot_rom_path, std::string_view rom_path, std::string_view sram_path) {
    save_boot_rom(boot_rom_path);

    mbc = std::make_unique<plain_rom>();
    mbc->load_rom_from_file(rom_path);
}