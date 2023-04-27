// File: timer.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_TIMER_HPP
#define SEMESTER_PROJECT_TIMER_HPP

#include "../cpu/cpu_interrupt_typedef.hpp"
#include "../utility.hpp"
//TODO
class timer {
    static constexpr int speeds [4] = {1024, 16, 64, 256};

    byte divider = 0;
    byte counter = 0;
    byte modulo = 0;
    byte control = 0;

    bool get_counter_enabled() const { return utility::get_bit(control, 2); };
    byte get_counter_speed() const { return speeds[control & 0b11]; };

    interrupt_callback request_cpu_interrupt;
public:
    timer(interrupt_callback callback) : request_cpu_interrupt(callback) {};
    void run_m_cycle();

    byte read_divider() const { return divider; };
    byte read_counter() const { return counter; };
    byte read_modulo() const { return modulo; };
    byte read_control() const { return control; };

    void write_divider(byte value) { divider = 0; };
    void write_counter(byte value) { counter = value; };
    void write_modulo(byte value) { modulo = value; };
    void write_control(byte value) { control = value; };
};

#endif //SEMESTER_PROJECT_TIMER_HPP
