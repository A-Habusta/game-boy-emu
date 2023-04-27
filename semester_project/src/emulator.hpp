// File: emulator.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_EMULATOR_HPP
#define SEMESTER_PROJECT_EMULATOR_HPP

#include <functional>
#include <chrono>
#include <array>

#include "cpu/central_processing_unit.hpp"
#include "hardware/cartridge.hpp"
#include "hardware/timer.hpp"
#include "hardware/apu.hpp"
#include "hardware/ram.hpp"
#include "hardware/ppu.hpp"

#include "utility.hpp"


namespace emulator {
    // Used as exception by the STOP instruction
    class stop {};

    constexpr int t_cycles_per_m_cycle = 4;

    constexpr int t_cycle_frequency = 4194304; //4MHz
    constexpr int m_cycle_frequency = t_cycle_frequency / t_cycles_per_m_cycle; //1MHz
    constexpr int m_cycles_per_frame = 70224;

    constexpr int frame_frequency = m_cycle_frequency / m_cycles_per_frame; //59.7Hz
    constexpr int ns_per_frame = 1000000000 / frame_frequency;

    class emulator {
        class memory_map {
        public:
            memory_map(emulator& emulator) : emu_ref(emulator) { }

            byte read_from_address(word address);
            void write_to_address(word address, byte value);

            void perform_dma_cycle() {
                for (int i = 0; i < t_cycles_per_m_cycle; ++i) {
                    if (dma_bytes_left-- > 0) {
                        byte dest_oam_addr = dma_bytes_copied_amount - dma_bytes_left - 1;

                        byte value = read_memory(dma_source_address + dest_oam_addr);
                        emu_ref.ppu.write_oam_dma(dest_oam_addr, value);
                    }
                }
            }
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

            static constexpr int unusable_end_address = 0xFEFF;

            static constexpr int io_start_address = 0xFF00;
            static constexpr int io_end_address = 0xFF7F;

            static constexpr int hram_start_address = 0xFF80;
            static constexpr int hram_end_address = 0xFFFE;

            static constexpr int interrupt_enable_address = 0xFFFF;

            static constexpr int dma_bytes_copied_amount = 0xA0;

            void start_dma(byte upper_address_byte) {
                dma_bytes_left = dma_bytes_copied_amount;
                dma_source_address = utility::get_word_from_byte(upper_address_byte, 0);
            }

            int dma_bytes_left{0};
            word dma_source_address{};
            bool is_dma_active() { return dma_bytes_left > 0; }

            emulator& emu_ref;

            byte read_memory(word address);
            void write_memory(word address, byte value);

            byte read_io(word address);
            void write_io(word address, byte value);
        };

        using clock = std::chrono::steady_clock;
        using time_point = clock::time_point;
        using duration = clock::duration;

        static constexpr duration frame_duration = std::chrono::nanoseconds(ns_per_frame);

        int cycle_counter;
        time_point last_frame_time_point;

        central_processing_unit::cpu cpu;
        timer emulated_timer;
        pixel_processing_unit::ppu ppu;
        audio_processing_unit::apu apu;
        cartridge cart;
        random_access_memory::ram ram;

        memory_map memory;

        byte joypad_register;

        bool is_stopped{false};

        byte read_with_cycling(word address);
        void write_with_cycling(word address, byte value);
        void run_machine_cycle();

        bool handle_input();

        void sleep_if_frame_time_too_short(time_point frame_current_time);

    public:
        emulator(SDL_Renderer *renderer);
        void execute_cpu() {
            try {
                cpu.execute();
            }
            catch (const stop&) {
                is_stopped = true;
                emulated_timer.write_divider(0);
            }
        }
    };
}

#endif //SEMESTER_PROJECT_EMULATOR_HPP
