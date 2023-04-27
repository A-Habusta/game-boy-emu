// File: emulator.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "cpu/central_processing_unit.hpp"
#include "hardware/cartridge.hpp"
#include "hardware/timer.hpp"
#include "hardware/ppu.hpp"
#include "hardware/apu.hpp"
#include "emulator.hpp"
#include "utility.hpp"

namespace emulator {

    byte emulator::read_with_cycling(word address) {
        byte value = memory.read_from_address(address);
        run_machine_cycle();
        return value;
    }
    void emulator::write_with_cycling(word address, byte value) {
        run_machine_cycle();
        memory.write_to_address(address, value);
    }

    void emulator::memory_map::write_to_address(word address, byte value) {
        if (address <= rom_end_address) {
            emulator_ref.cart.write_rom(address - rom_start_address, value);
        }
        else if (address <= vram_end_address) {
            emulator_ref.ppu.write_vram(address - vram_start_address, value);
        }
        else if (address <= sram_end_address) {
            emulator_ref.cart.write_ram(address - sram_start_address, value);
        }
        else if (address <= wram_end_address) {
            emulator_ref.ram.write_wram(address - wram_start_address, value);
        }
        else if (address <= echo_end_address) {
            emulator_ref.ram.write_wram(address - echo_start_address, value);
        }
        else if (address <= oam_end_address) {
            emulator_ref.ppu.write_oam(address - oam_start_address, value);
        }
        else if (address <= unusable_end_address) {
            // do nothing
        }
        else if (address <= io_end_address) {
            write_io(address - io_start_address, value);
        }
        else if (address <= hram_end_address) {
            emulator_ref.ram.write_hram(address - hram_start_address, value);
        }
        else if (address == interrupt_enable_address) {
            emulator_ref.cpu.interrupt_enable_register = value;
        }
    }

    byte emulator::memory_map::read_from_address(word address) {
            if (address <= rom_end_address) {
                emulator_ref.cart.read_rom(address - rom_start_address);
            }
            else if (address <= vram_end_address) {
                return emulator_ref.ppu.read_vram(address - vram_start_address);
            }
            else if (address <= sram_end_address) {
                return emulator_ref.cart.read_ram(address - sram_start_address);
            }
            else if (address <= wram_end_address) {
                return emulator_ref.ram.read_wram(address - wram_start_address);
            }
            else if (address <= echo_end_address) {
                return emulator_ref.ram.read_wram(address - echo_start_address);
            }
            else if (address <= oam_end_address) {
                return emulator_ref.ppu.read_oam(address - oam_start_address);
            }
            else if (address <= unusable_end_address) {
                // do nothing
            }
            else if (address <= io_end_address) {
                return read_io(address - io_start_address);
            }
            else if (address <= hram_end_address) {
                return emulator_ref.ram.read_hram(address - hram_start_address);
            }
            else if (address == interrupt_enable_address) {
                return emulator_ref.cpu.interrupt_enable_register;
            }
    }

    void emulator::memory_map::setup_io() {
        //TODO
        return;
    }
}