// File: cartridge.hpp
//
// Created by Adrian Habusta on 26.04.2023
//

#ifndef SEMESTER_PROJECT_CARTRIDGE_HPP
#define SEMESTER_PROJECT_CARTRIDGE_HPP

#include <string_view>
#include <memory>

#include "cartridge_memory_controllers.hpp"
#include "../utility.hpp"


class cartridge {
    static constexpr int boot_rom_size = 0x100;

    std::unique_ptr<cartridge_mbc> mbc;

    bool boot_rom_enabled = true;
    byte boot_rom_register{};

    void save_boot_rom(std::string_view boot_rom_path) {
        utility::read_file(boot_rom_path, boot_rom, boot_rom_size, "Failed to load boot rom");
    }

    byte boot_rom[boot_rom_size] {0};
public:
    cartridge(std::string_view boot_rom_path, std::string_view rom_path, std::string_view sram_path);

    byte read_boot_rom_disable() const { return boot_rom_register; }
    void write_boot_rom_disable(byte value) {
        if (value > 0)
            boot_rom_enabled = false;

        boot_rom_register = value;
    }

    byte read_rom(word address) {
        if (boot_rom_enabled && address < boot_rom_size)
            return boot_rom[address];

        return mbc->read_rom(address);
    }

    byte read_ram(word address) {
        return mbc->read_ram(address);
    }

    // This is used for controlling the MBC
    void write_rom(word address, byte value) {
        mbc->write_rom(address, value);
    }

    void write_ram(word address, byte value) {
        mbc->write_ram(address, value);
    }
};

#endif //SEMESTER_PROJECT_CARTRIDGE_HPP
