// File: memory.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "memory.hpp"

namespace memory {
    byte memory_map::read_with_cycling(word address);
    void memory_map::write_with_cycling(word address, byte value);
}