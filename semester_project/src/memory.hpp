// File: memory.hpp
//
// Created by Adrian Habusta on 19.04.2023
//

#ifndef SEMESTER_PROJECT_MEMORY_HPP
#define SEMESTER_PROJECT_MEMORY_HPP

#include "utility.hpp"

namespace memory {
    struct memory_map {
        byte read_with_cycling(word address);
        void write_with_cycling(word address, byte value);
    };
}

#endif //SEMESTER_PROJECT_MEMORY_HPP
