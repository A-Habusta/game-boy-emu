// File: cpu_interrupt_typedef.hpp
//
// Created by Adrian Habusta on 24.04.2023
//

#ifndef SEMESTER_PROJECT_CPU_INTERRUPT_TYPEDEF_HPP
#define SEMESTER_PROJECT_CPU_INTERRUPT_TYPEDEF_HPP

#include <functional>

using interrupt_handler = std::function<void()>;

#endif //SEMESTER_PROJECT_CPU_INTERRUPT_TYPEDEF_HPP
