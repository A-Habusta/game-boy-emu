// File: timer.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_TIMER_HPP
#define SEMESTER_PROJECT_TIMER_HPP

namespace timer {
class timer {
        byte divider = 0;
        byte counter = 0;
        byte modulo = 0;
        byte control = 0;
    public:
        timer()
        void run_m_cycle();
    };
}

#endif //SEMESTER_PROJECT_TIMER_HPP
