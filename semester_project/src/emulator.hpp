// File: emulator.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_EMULATOR_HPP
#define SEMESTER_PROJECT_EMULATOR_HPP

#include <functional>

#include "cpu/central_processing_unit.hpp"
#include "hardware/cartridge.hpp"
#include "hardware/timer.hpp"
#include "hardware/apu.hpp"
#include "hardware/ram.hpp"
#include "hardware/ppu.hpp"

#include "utility.hpp"


namespace emulator {
    class emulator {
        class memory_map {
        public:
            memory_map(emulator& emulator) : emulator_ref(emulator) {}

            byte read_from_address(word address);
            void write_to_address(word address, byte value);
        private:
            static constexpr int rom_start_address = 0x0000;
            static constexpr int rom_end_address = 0x7FFF;

            static constexpr int vram_start_address = 0x8000;
            static constexpr int vram_end_address = 0x9FFF;

            static constexpr int sram_start_address = 0xA000;
            static constexpr int sram_end_address = 0xBFFF;

            static constexpr int wram_start_address = 0xC000;
            static constexpr int wram_end_address = 0xDFFF;
            static constexpr int echo_start_address = 0xE000;
            static constexpr int echo_end_address = 0xFDFF;

            static constexpr int oam_start_address = 0xFE00;
            static constexpr int oam_end_address = 0xFE9F;

            static constexpr int unusable_start_address = 0xFEA0;
            static constexpr int unusable_end_address = 0xFEFF;

            static constexpr int io_start_address = 0xFF00;
            static constexpr int io_end_address = 0xFF7F;

            static constexpr int hram_start_address = 0xFF80;
            static constexpr int hram_end_address = 0xFFFE;

            static constexpr int interrupt_enable_address = 0xFFFF;

            emulator& emulator_ref;

            void setup_io();
            byte read_io(word address);
            void write_io(word address, byte value);
        };

        random_access_memory::ram ram;
        central_processing_unit::cpu cpu;
        timer emulated_timer;
        pixel_processing_unit::ppu ppu;
        audio_processing_unit::apu apu;
        cartridge cart;

        memory_map memory;

        byte read_with_cycling(word address);
        void write_with_cycling(word address, byte value);
    public:
        emulator();
        void run_machine_cycle();
    };

    // Used as exception by the STOP instruction
    class stop {

    };
}

#endif //SEMESTER_PROJECT_EMULATOR_HPP
