// File: cartridge_memory_controllers.hpp
//
// Created by Adrian Habusta on 28.04.2023
//

#ifndef SEMESTER_PROJECT_CARTRIDGE_MEMORY_CONTROLLERS_HPP
#define SEMESTER_PROJECT_CARTRIDGE_MEMORY_CONTROLLERS_HPP

#include <string_view>
#include "../utility.hpp"

class cartridge_mbc {
protected:
    cartridge_mbc() = default;
public:
    virtual void load_rom_from_file(std::string_view path) = 0;
    virtual void load_sram_from_file(std::string_view path) {};

    virtual byte read_rom(word address) const = 0;
    virtual byte read_ram(word address) const { return utility::undefined_byte; };

    virtual void write_rom(word address, byte value) {};
    virtual void write_ram(word address, byte value) {};

    virtual ~cartridge_mbc() = default;
};

class plain_rom : public cartridge_mbc {
    static constexpr int rom_size = 0x8000;
    byte rom[rom_size] = {0};

public:
    void load_rom_from_file(std::string_view path) override;

    byte read_rom(word address) const override { return rom[address]; };
};

#endif //SEMESTER_PROJECT_CARTRIDGE_MEMORY_CONTROLLERS_HPP
