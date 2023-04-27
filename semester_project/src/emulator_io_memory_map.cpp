// File: emulator_io_memory_map.hpp
//
// Created by Adrian Habusta on 27.04.2023
//

#include "utility.hpp"
#include "emulator.hpp"

namespace emulator {
    byte emulator::memory_map::read_io(word address) {
        byte truncated_address = address;
        switch (truncated_address) {
            // Joypad register
            case 0x00: return emu_ref.joypad_register;

            // Serial communication registers NOT IMPLEMENTED

            // Timer registers
            case 0x04: return emu_ref.emulated_timer.read_divider();
            case 0x05: return emu_ref.emulated_timer.read_counter();
            case 0x06: return emu_ref.emulated_timer.read_modulo();
            case 0x07: return emu_ref.emulated_timer.read_control();

            // Interrupt flag
            case 0x0F: return emu_ref.cpu.interrupt_requested_register;

            // Sound registers NOT IMPLEMENTED FF10 - FF3F

            // LCD registers
            case 0x40: return emu_ref.ppu.read_lcd_control();
            case 0x41: return emu_ref.ppu.read_lcd_status();
            case 0x42: return emu_ref.ppu.read_scroll_y();
            case 0x43: return emu_ref.ppu.read_scroll_x();
            case 0x44: return emu_ref.ppu.read_lcd_y();
            case 0x45: return emu_ref.ppu.read_lcd_y_compare();
            case 0x46: return emu_ref.ppu.read_dma_transfer();
            case 0x47: return emu_ref.ppu.read_bg_palette();
            case 0x48: return emu_ref.ppu.read_sprite_palette_0();
            case 0x49: return emu_ref.ppu.read_sprite_palette_1();
            case 0x4A: return emu_ref.ppu.read_window_y();
            case 0x4B: return emu_ref.ppu.read_window_x();

            // Boot ROM disable NOT IMPLEMENTED

            default: return utility::undefined_byte;
        }
    }

    void emulator::memory_map::write_io(word address, byte value) {
        byte truncated_address = address;
        switch (truncated_address) {
            // Joypad register
            case 0x00: emu_ref.joypad_register = value; break;

                // Serial communication registers NOT IMPLEMENTED

                // Timer registers
            case 0x04: emu_ref.emulated_timer.write_divider(value); break;
            case 0x05: emu_ref.emulated_timer.write_counter(value); break;
            case 0x06: emu_ref.emulated_timer.write_modulo(value); break;
            case 0x07: emu_ref.emulated_timer.write_control(value); break;

                // Interrupt flag
            case 0x0F: emu_ref.cpu.interrupt_requested_register = value; break;

                // Sound registers NOT IMPLEMENTED FF10 - FF3F

                //LCD registers
            case 0x40: emu_ref.ppu.write_lcd_control(value); break;
            case 0x41: emu_ref.ppu.write_lcd_status(value); break;
            case 0x42: emu_ref.ppu.write_scroll_y(value); break;
            case 0x43: emu_ref.ppu.write_scroll_x(value); break;
            case 0x44: emu_ref.ppu.write_lcd_y(value); break;
            case 0x45: emu_ref.ppu.write_lcd_y_compare(value); break;
                // DMA TRANSFER
            case 0x46: start_dma(value); break;
            case 0x47: emu_ref.ppu.write_bg_palette(value); break;
            case 0x48: emu_ref.ppu.write_sprite_palette_0(value); break;
            case 0x49: emu_ref.ppu.write_sprite_palette_1(value); break;
            case 0x4A: emu_ref.ppu.write_window_y(value); break;
            case 0x4B: emu_ref.ppu.write_window_x(value); break;

            default: break;
        }
    }
}