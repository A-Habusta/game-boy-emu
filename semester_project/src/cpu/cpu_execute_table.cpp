// File: cpu_execute_table.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../utility.hpp"

namespace central_processing_unit {
    void cpu::execute_instruction() {
        switch(cached_instruction) {
            //load register8 <- immediate
            case 0x3E: load(registers::half_register_name::A, read_byte_at_pc_with_increment()); break;
            case 0x06: load(registers::half_register_name::B, read_byte_at_pc_with_increment()); break;
            case 0x0E: load(registers::half_register_name::C, read_byte_at_pc_with_increment()); break;
            case 0x16: load(registers::half_register_name::D, read_byte_at_pc_with_increment()); break;
            case 0x1E: load(registers::half_register_name::E, read_byte_at_pc_with_increment()); break;
            case 0x26: load(registers::half_register_name::H, read_byte_at_pc_with_increment()); break;
            case 0x2E: load(registers::half_register_name::L, read_byte_at_pc_with_increment()); break;

            //load register8 <- register8
            case 0x7F: load(registers::half_register_name::A, registers.half.A); break;
            case 0x78: load(registers::half_register_name::A, registers.half.B); break;
            case 0x79: load(registers::half_register_name::A, registers.half.C); break;
            case 0x7A: load(registers::half_register_name::A, registers.half.D); break;
            case 0x7B: load(registers::half_register_name::A, registers.half.E); break;
            case 0x7C: load(registers::half_register_name::A, registers.half.H); break;
            case 0x7D: load(registers::half_register_name::A, registers.half.L); break;

            case 0x40: load(registers::half_register_name::B, registers.half.B); break;
            case 0x41: load(registers::half_register_name::B, registers.half.C); break;
            case 0x42: load(registers::half_register_name::B, registers.half.D); break;
            case 0x43: load(registers::half_register_name::B, registers.half.E); break;
            case 0x44: load(registers::half_register_name::B, registers.half.H); break;
            case 0x45: load(registers::half_register_name::B, registers.half.L); break;

            case 0x48: load(registers::half_register_name::C, registers.half.B); break;
            case 0x49: load(registers::half_register_name::C, registers.half.C); break;
            case 0x4A: load(registers::half_register_name::C, registers.half.D); break;
            case 0x4B: load(registers::half_register_name::C, registers.half.E); break;
            case 0x4C: load(registers::half_register_name::C, registers.half.H); break;
            case 0x4D: load(registers::half_register_name::C, registers.half.L); break;

            case 0x50: load(registers::half_register_name::D, registers.half.B); break;
            case 0x51: load(registers::half_register_name::D, registers.half.C); break;
            case 0x52: load(registers::half_register_name::D, registers.half.D); break;
            case 0x53: load(registers::half_register_name::D, registers.half.E); break;
            case 0x54: load(registers::half_register_name::D, registers.half.H); break;
            case 0x55: load(registers::half_register_name::D, registers.half.L); break;

            case 0x58: load(registers::half_register_name::E, registers.half.B); break;
            case 0x59: load(registers::half_register_name::E, registers.half.C); break;
            case 0x5A: load(registers::half_register_name::E, registers.half.D); break;
            case 0x5B: load(registers::half_register_name::E, registers.half.E); break;
            case 0x5C: load(registers::half_register_name::E, registers.half.H); break;
            case 0x5D: load(registers::half_register_name::E, registers.half.L); break;

            case 0x60: load(registers::half_register_name::H, registers.half.B); break;
            case 0x61: load(registers::half_register_name::H, registers.half.C); break;
            case 0x62: load(registers::half_register_name::H, registers.half.D); break;
            case 0x63: load(registers::half_register_name::H, registers.half.E); break;
            case 0x64: load(registers::half_register_name::H, registers.half.H); break;
            case 0x65: load(registers::half_register_name::H, registers.half.L); break;

            case 0x68: load(registers::half_register_name::L, registers.half.B); break;
            case 0x69: load(registers::half_register_name::L, registers.half.C); break;
            case 0x6A: load(registers::half_register_name::L, registers.half.D); break;
            case 0x6B: load(registers::half_register_name::L, registers.half.E); break;
            case 0x6C: load(registers::half_register_name::L, registers.half.H); break;
            case 0x6D: load(registers::half_register_name::L, registers.half.L); break;

            //load register8 <- indirect
            case 0x46: load(registers::half_register_name::B, read_byte(registers.whole.HL)); break;
            case 0x4E: load(registers::half_register_name::C, read_byte(registers.whole.HL)); break;
            case 0x56: load(registers::half_register_name::D, read_byte(registers.whole.HL)); break;
            case 0x5E: load(registers::half_register_name::E, read_byte(registers.whole.HL)); break;
            case 0x66: load(registers::half_register_name::H, read_byte(registers.whole.HL)); break;
            case 0x6E: load(registers::half_register_name::L, read_byte(registers.whole.HL)); break;

            case 0x0A: load(registers::half_register_name::A, read_byte(registers.whole.BC)); break;
            case 0x1A: load(registers::half_register_name::A, read_byte(registers.whole.DE)); break;
            case 0x7E: load(registers::half_register_name::A, read_byte(registers.whole.HL)); break;
            case 0xFA: load(registers::half_register_name::A, read_byte(read_word_at_pc_with_increment())); break;

            //load indirect <- register8
            case 0x70: load(registers.whole.HL, registers.half.B); break;
            case 0x71: load(registers.whole.HL, registers.half.C); break;
            case 0x72: load(registers.whole.HL, registers.half.D); break;
            case 0x73: load(registers.whole.HL, registers.half.E); break;
            case 0x74: load(registers.whole.HL, registers.half.H); break;
            case 0x75: load(registers.whole.HL, registers.half.L); break;

            case 0x02: load(registers.whole.BC, registers.half.A); break;
            case 0x12: load(registers.whole.DE, registers.half.A); break;
            case 0x77: load(registers.whole.HL, registers.half.A); break;
            case 0xEA: load(read_word_at_pc_with_increment(), registers.half.A); break;

            //load indirect <- immediate
            case 0x36: load(registers.whole.HL, read_byte_at_pc_with_increment()); break;

            //load high page memory <- A
            case 0xE0: load(HIGH_PAGE | read_byte_at_pc_with_increment(), registers.half.A); break;
            case 0xE2: load(HIGH_PAGE | read_byte(registers.half.C), registers.half.A); break;

            //load A <- high page memory
            case 0xF0: load(registers::half_register_name::A, read_byte(HIGH_PAGE | read_byte_at_pc_with_increment())); break;
            case 0xF2: load(registers::half_register_name::A, read_byte(HIGH_PAGE | registers.half.C)); break;

            //load A <- indirect w inc/dec
            case 0x2A: load(registers::half_register_name::A, read_byte(registers.whole.HL++)); break;
            case 0x3A: load(registers::half_register_name::A, read_byte(registers.whole.HL--)); break;

            //load indirect w inc/dec <- A
            case 0x22: load(registers.whole.HL++, registers.half.A); break;
            case 0x32: load(registers.whole.HL--, registers.half.A); break;

            //load register16 <- immediate
            case 0x01: load(registers::whole_register_name::BC, read_word_at_pc_with_increment()); break;
            case 0x11: load(registers::whole_register_name::DE, read_word_at_pc_with_increment()); break;
            case 0x21: load(registers::whole_register_name::HL, read_word_at_pc_with_increment()); break;
            case 0x31: load(registers::whole_register_name::SP, read_word_at_pc_with_increment()); break;

            //load indirect <- SP
            case 0x08: load(read_word_at_pc_with_increment(), registers.whole.SP); break;
            //load SP <- HL
            case 0xF9: run_phantom_cycle(); load(registers::register16::SP, registers.whole.HL); break;

            //TODO
            //load HL <- SP + immediate

            //pop
            case 0xC1: pop(registers::whole_register_name::BC); break;
            case 0xD1: pop(registers::whole_register_name::DE); break;
            case 0xE1: pop(registers::whole_register_name::HL); break;
            case 0xF1: pop(registers::whole_register_name::AF); break;

            //push
            case 0xC5: push(registers::whole_register_name::BC); break;
            case 0xD5: push(registers::whole_register_name::DE); break;
            case 0xE5: push(registers::whole_register_name::HL); break;
            case 0xF5: push(registers::whole_register_name::AF); break;
        }
    }
}
