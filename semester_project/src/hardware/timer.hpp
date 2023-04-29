// File: timer.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_TIMER_HPP
#define SEMESTER_PROJECT_TIMER_HPP

#include <utility>

#include <utility>

#include "../cpu/cpu_interrupt_typedef.hpp"
#include "../utility.hpp"
//TODO
class timer {
    static constexpr int system_frequency = 4194304; //4MhZ
    static constexpr int divider_frequency = 16384;
    static constexpr int cycles_per_div_increment = system_frequency / divider_frequency;

    static constexpr int cycles_per_m_cycle = 4;

    static constexpr int speeds [4] = {1024, 16, 64, 256};

    int divider_counter = 0;
    int main_counter = 0;

    byte divider = 0;
    byte counter = 0;
    byte modulo = 0;
    byte control = 0;

    [[nodiscard]] bool is_counter_enabled() const { return utility::get_bit(control, 2); };
    [[nodiscard]] int get_counter_speed() const { return speeds[control & 0b11]; };

    void progress_divider();
    void progress_main();

    interrupt_callback request_cpu_interrupt;
public:
    explicit timer(interrupt_callback callback) : request_cpu_interrupt(std::move(std::move(callback))) {};
    void run_machine_cycle() {
        progress_divider();
        progress_main();
    }

    [[nodiscard]] byte read_divider() const { return divider; };
    [[nodiscard]] byte read_counter() const { return counter; };
    [[nodiscard]] byte read_modulo() const { return modulo; };
    [[nodiscard]] byte read_control() const { return control; };

    // value is unused here, writing to div resets it
    void write_divider(byte value [[maybe_unused]]) { divider = 0; };
    void write_counter(byte value) { counter = value; };
    void write_modulo(byte value) { modulo = value; };
    void write_control(byte value) { control = value; };
};

#endif //SEMESTER_PROJECT_TIMER_HPP
