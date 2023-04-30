// File: emulator.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_EMULATOR_HPP
#define SEMESTER_PROJECT_EMULATOR_HPP

#include <string_view>
#include <functional>
#include <chrono>
#include <array>

#include "cpu/central_processing_unit.hpp"
#include "hardware/cartridge.hpp"
#include "hardware/joypad.hpp"
#include "hardware/timer.hpp"
#include "hardware/apu.hpp"
#include "hardware/ram.hpp"
#include "hardware/ppu.hpp"

#include "utility.hpp"


namespace emulator {
    // Used as exception by the STOP instruction
    class stop {};
    class exit {};

    constexpr std::size_t t_cycles_per_m_cycle = 4;

    constexpr std::size_t t_cycle_frequency = 4194304; //4MHz
    constexpr std::size_t m_cycle_frequency = t_cycle_frequency / t_cycles_per_m_cycle; //1MHz
    constexpr std::size_t m_cycles_per_frame = 17566;

    constexpr double frame_frequency = m_cycle_frequency / m_cycles_per_frame; //59.7Hz
    constexpr double ns_per_frame = 1000000000 / frame_frequency;

    class emulator {
        class memory_map {
        public:
            explicit memory_map(emulator& emulator) : emu_ref(emulator) { }

            byte read_from_address(word address);
            void write_to_address(word address, byte value);

            void perform_dma_cycle() {
                for (unsigned i = 0; i < t_cycles_per_m_cycle; ++i) {
                    if (dma_bytes_left > 0) {
                        --dma_bytes_left;

                        byte dest_oam_addr = dma_bytes_copied_amount - dma_bytes_left - 1;

                        byte value = read_memory(dma_source_address + dest_oam_addr);
                        emu_ref.ppu.write_oam_dma(dest_oam_addr, value);
                    }
                }
            }
        private:
            enum {
                rom_start_address = 0x0000,
                rom_end_address = 0x7FFF,

                vram_start_address = 0x8000,
                vram_end_address = 0x9FFF,

                sram_start_address = 0xA000,
                sram_end_address = 0xBFFF,

                wram_start_address = 0xC000,
                wram_end_address = 0xDFFF,

                echo_start_address = 0xE000,
                echo_end_address = 0xFDFF,

                oam_start_address = 0xFE00,
                oam_end_address = 0xFE9F,

                unusable_end_address = 0xFEFF,

                io_start_address = 0xFF00,
                io_end_address = 0xFF7F,

                hram_start_address = 0xFF80,
                hram_end_address = 0xFFFE,

                interrupt_enable_address = 0xFFFF,

                dma_bytes_copied_amount = 0xA0
            };


            void start_dma(byte upper_address_byte) {
                dma_bytes_left = dma_bytes_copied_amount;
                dma_source_address = utility::get_word_from_bytes(0, upper_address_byte);
            }

            int dma_bytes_left{0};
            word dma_source_address{};
            [[nodiscard]] bool is_dma_active() const { return dma_bytes_left > 0; }

            emulator& emu_ref;

            byte read_memory(word address);
            void write_memory(word address, byte value);

            byte read_io(word address);
            void write_io(word address, byte value);
        };

        using clock = std::chrono::steady_clock;
        using time_point = clock::time_point;
        using duration = clock::duration;

        static constexpr duration frame_duration = std::chrono::nanoseconds((std::size_t)ns_per_frame);

        std::size_t cycle_counter{};
        time_point last_frame_time_point;

        central_processing_unit::cpu cpu;
        timer emulated_timer;
        pixel_processing_unit::ppu ppu;
        joypad buttons;
        audio_processing_unit::apu apu;
        cartridge cart;
        random_access_memory::ram ram;

        memory_map memory;

        byte read_with_cycling(word address);
        void write_with_cycling(word address, byte value);
        void run_machine_cycle();

        void sleep_if_frame_time_too_short(time_point frame_current_time);

        void stop_loop();


    public:
        emulator(SDL_Renderer *renderer, std::string_view boot_rom_path, std::string_view rom_path, std::string_view
        sram_path);

        void execute_cpu() {
            try {
                cpu.execute();
            }
            catch (const stop&) {
                emulated_timer.write_divider(0);
                stop_loop();
            }
        }
    };
}

#endif //SEMESTER_PROJECT_EMULATOR_HPP
