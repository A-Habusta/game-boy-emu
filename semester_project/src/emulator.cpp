// File: emulator.cpp
//
// Created by Adrian Habusta on 22.04.2023
//

void emulator::run_cycle() {
    ppu.run_m_cycle();
    apu.run_m_cycle();
    timer.run_m_cycle();
}