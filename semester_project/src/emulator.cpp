// File: emulator.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include <chrono>
#include <thread>

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
        if (is_dma_active() && address < hram_start_address)
            return;

        write_memory(address, value);
    }

    byte emulator::memory_map::read_from_address(word address) {
        if (is_dma_active() && address < hram_start_address)
            return utility::undefined_byte;

        return read_memory(address);
    }

    void emulator::memory_map::write_memory(word address, byte value) {
        if (address <= rom_end_address) {
            emu_ref.cart.write_rom(address - rom_start_address, value);
        }
        else if (address <= vram_end_address) {
            emu_ref.ppu.write_vram(address - vram_start_address, value);
        }
        else if (address <= sram_end_address) {
            emu_ref.cart.write_ram(address - sram_start_address, value);
        }
        else if (address <= wram_end_address) {
            emu_ref.ram.write_wram(address - wram_start_address, value);
        }
        else if (address <= echo_end_address) {
            emu_ref.ram.write_wram(address - echo_start_address, value);
        }
        else if (address <= oam_end_address) {
            emu_ref.ppu.write_oam(address - oam_start_address, value);
        }
        else if (address <= unusable_end_address) {
            // do nothing
        }
        else if (address <= io_end_address) {
            write_io(address - io_start_address, value);
        }
        else if (address <= hram_end_address) {
            emu_ref.ram.write_hram(address - hram_start_address, value);
        }
        else if (address == interrupt_enable_address) {
            emu_ref.cpu.interrupt_enable_register = value;
        }
    }

    byte emulator::memory_map::read_memory(word address) {
        if (address <= rom_end_address) {
            return emu_ref.cart.read_rom(address - rom_start_address);
        }
        else if (address <= vram_end_address) {
            return emu_ref.ppu.read_vram(address - vram_start_address);
        }
        else if (address <= sram_end_address) {
            return emu_ref.cart.read_ram(address - sram_start_address);
        }
        else if (address <= wram_end_address) {
            return emu_ref.ram.read_wram(address - wram_start_address);
        }
        else if (address <= echo_end_address) {
            return emu_ref.ram.read_wram(address - echo_start_address);
        }
        else if (address <= oam_end_address) {
            return emu_ref.ppu.read_oam(address - oam_start_address);
        }
        else if (address <= unusable_end_address) {
            // do nothing
        }
        else if (address <= io_end_address) {
            return read_io(address - io_start_address);
        }
        else if (address <= hram_end_address) {
            return emu_ref.ram.read_hram(address - hram_start_address);
        }
        else if (address == interrupt_enable_address) {
            return emu_ref.cpu.interrupt_enable_register;
        }
    }

    emulator::emulator(SDL_Renderer* renderer)
        : cpu([this](word addr){ return read_with_cycling(addr); },
              [this](word addr, byte value){ write_with_cycling(addr, value); },
              [this](){ run_machine_cycle(); }),
          emulated_timer([this]{ cpu.request_timer_interrupt(); }),
          ppu(renderer, [this]{ cpu.request_lcd_stat_interrupt(); },
                        [this]{ cpu.request_v_blank_interrupt(); } ),
          buttons([this]{ cpu.request_joypad_interrupt(); } ),
          apu(),
          cart(),
          ram(),
          memory(*this) {
    }

    void emulator::sleep_if_frame_time_too_short(time_point time) {
        auto current_frame_duration = time - last_frame_time_point;
        if (current_frame_duration < frame_duration) {
            // This might not be too precise, but it allows the cpu to rest a bit and not be consumed all the time
            std::this_thread::sleep_for(frame_duration - current_frame_duration);
        }
    }

    void emulator::run_machine_cycle() {
        memory.perform_dma_cycle();
        ppu.run_machine_cycle();
        //apu.run_machine_cycle();
        emulated_timer.run_machine_cycle();

        cycle_counter++;

        if (cycle_counter >= m_cycles_per_frame) {
            buttons.handle_input();

            cycle_counter = 0;

            auto time = clock::now();
            sleep_if_frame_time_too_short(time);
            last_frame_time_point = time;
        }
    }
}