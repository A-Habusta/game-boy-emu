// File: timer.cpp
//
// Created by Adrian Habusta on 24.04.2023
//

#include "timer.hpp"


void timer::progress_divider() {
    divider_counter += cycles_per_m_cycle;

    if (divider_counter >= cycles_per_div_increment) {
        divider_counter -= cycles_per_div_increment;
        divider++;
    }
}

void timer::progress_main() {
    if (!get_counter_enabled())
        return;

    main_counter += cycles_per_m_cycle;

    int counter_limit = get_counter_speed();
    if (main_counter >= counter_limit) {
        main_counter -= counter_limit;
        counter++;

        if (counter == 0) {
            counter = modulo;
            request_cpu_interrupt(interrupt_type::timer);
        }
    }
}