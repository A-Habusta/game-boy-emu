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
            case 0xF9: run_phantom_cycle(); load(registers::whole_register_name::SP, registers.whole.HL); break;

            //TODO
            //load HL <- SP + immediate
            case 0xF8: load(registers::whole_register_name::HL, add_to_sp(read_byte_at_program_counter_with_increment)); break;

            //pop
            case 0xC1: pop(registers::whole_register_name::BC); break;
            case 0xD1: pop(registers::whole_register_name::DE); break;
            case 0xE1: pop(registers::whole_register_name::HL); break;
            case 0xF1: pop(registers::whole_register_name::AF); break;

            //push
            case 0xF5: push(registers.whole.AF); break;
            case 0xC5: push(registers.whole.BC); break;
            case 0xD5: push(registers.whole.DE); break;
            case 0xE5: push(registers.whole.HL); break;

            //add
            case 0x87: add(registers.half.A); break;
            case 0x80: add(registers.half.B); break;
            case 0x81: add(registers.half.C); break;
            case 0x82: add(registers.half.D); break;

            case 0x83: add(registers.half.E); break;
            case 0x84: add(registers.half.H); break;
            case 0x85: add(registers.half.L); break;
            case 0x86: add(read_byte(registers.whole.HL)); break;
            case 0xC6: add(read_byte_at_pc_with_increment()); break;

            //add16
            case 0x09: add16(registers.whole.BC); break;
            case 0x19: add16(registers.whole.DE); break;
            case 0x29: add16(registers.whole.HL); break;
            case 0x39: add16(registers.whole.SP); break;

            //add16  SP + immediate
            // mildly cursed
            case 0xE8: registers.whole.SP = add_to_sp(read_byte_at_pc_with_increment); run_phantom_cycle(); break;

            //adc
            case 0x8F: add(registers.half.A, registers.read_carry_flag()); break;
            case 0x88: add(registers.half.B, registers.read_carry_flag()); break;
            case 0x89: add(registers.half.C, registers.read_carry_flag()); break;
            case 0x8A: add(registers.half.D, registers.read_carry_flag()); break;
            case 0x8B: add(registers.half.E, registers.read_carry_flag()); break;
            case 0x8C: add(registers.half.H, registers.read_carry_flag()); break;
            case 0x8D: add(registers.half.L, registers.read_carry_flag()); break;
            case 0x8E: add(read_byte(registers.whole.HL), registers.read_carry_flag()); break;
            case 0xCE: add(read_byte_at_pc_with_increment(), registers.read_carry_flag()); break;

            //sub
            case 0x97: sub(registers.half.A); break;
            case 0x90: sub(registers.half.B); break;
            case 0x91: sub(registers.half.C); break;
            case 0x92: sub(registers.half.D); break;
            case 0x93: sub(registers.half.E); break;
            case 0x94: sub(registers.half.H); break;
            case 0x95: sub(registers.half.L); break;
            case 0x96: sub(read_byte(registers.whole.HL)); break;
            case 0xD6: sub(read_byte_at_pc_with_increment()); break;

            //sbc
            case 0x9F: sub(registers.half.A, registers.read_carry_flag()); break;
            case 0x98: sub(registers.half.B, registers.read_carry_flag()); break;
            case 0x99: sub(registers.half.C, registers.read_carry_flag()); break;
            case 0x9A: sub(registers.half.D, registers.read_carry_flag()); break;
            case 0x9B: sub(registers.half.E, registers.read_carry_flag()); break;
            case 0x9C: sub(registers.half.H, registers.read_carry_flag()); break;
            case 0x9C: sub(registers.half.L, registers.read_carry_flag()); break;
            case 0x9E: sub(read_byte(registers.whole.HL), registers.read_carry_flag()); break;
            case 0xDE: sub(read_byte_at_pc_with_increment(), registers.read_carry_flag()); break;

            //and
            case 0xA7: and_(registers.half.A); break;
            case 0xA0: and_(registers.half.B); break;
            case 0xA1: and_(registers.half.C); break;
            case 0xA2: and_(registers.half.D); break;
            case 0xA3: and_(registers.half.E); break;
            case 0xA4: and_(registers.half.H); break;
            case 0xA5: and_(registers.half.L); break;
            case 0xA6: and_(read_byte(registers.whole.HL)); break;
            case 0xE6: and_(read_byte_at_pc_with_increment()); break;

            //or
            case 0xB7: or_(registers.half.A); break;
            case 0xB0: or_(registers.half.B); break;
            case 0xB1: or_(registers.half.C); break;
            case 0xB2: or_(registers.half.D); break;
            case 0xB3: or_(registers.half.E); break;
            case 0xB4: or_(registers.half.H); break;
            case 0xB5: or_(registers.half.L); break;
            case 0xB6: or_(read_byte(registers.whole.HL)); break;
            case 0xF6: or_(read_byte_at_pc_with_increment()); break;

            //xor
            case 0xAF: xor_(registers.half.A); break;
            case 0xA8: xor_(registers.half.B); break;
            case 0xA9: xor_(registers.half.C); break;
            case 0xAA: xor_(registers.half.D); break;
            case 0xAB: xor_(registers.half.E); break;
            case 0xAC: xor_(registers.half.H); break;
            case 0xAD: xor_(registers.half.L); break;
            case 0xAE: xor_(read_byte(registers.whole.HL)); break;
            case 0xEE: xor_(read_byte_at_pc_with_increment()); break;

            //cp
            case 0xBF: cp(registers.half.A); break;
            case 0xB8: cp(registers.half.B); break;
            case 0xB9: cp(registers.half.C); break;
            case 0xBA: cp(registers.half.D); break;
            case 0xBB: cp(registers.half.E); break;
            case 0xBC: cp(registers.half.H); break;
            case 0xBD: cp(registers.half.L); break;
            case 0xBE: cp(read_byte(registers.whole.HL)); break;
            case 0xFE: cp(read_byte_at_pc_with_increment()); break;

            //inc 8bit
            case 0x3C: inc(registers::half_register_name::A); break;
            case 0x04: inc(registers::half_register_name::B); break;
            case 0x0C: inc(registers::half_register_name::C); break;
            case 0x14: inc(registers::half_register_name::D); break;
            case 0x1C: inc(registers::half_register_name::E); break;
            case 0x24: inc(registers::half_register_name::H); break;
            case 0x2C: inc(registers::half_register_name::L); break;
            case 0x34: inc(registers.whole.HL); break;

            //dec 8bit
            case 0x3D: dec(registers::half_register_name::A); break;
            case 0x05: dec(registers::half_register_name::B); break;
            case 0x0D: dec(registers::half_register_name::C); break;
            case 0x15: dec(registers::half_register_name::D); break;
            case 0x1D: dec(registers::half_register_name::E); break;
            case 0x25: dec(registers::half_register_name::H); break;
            case 0x2D: dec(registers::half_register_name::L); break;
            case 0x35: dec(registers.whole.HL); break;

            //inc 16bit
            case 0x03: inc(registers::whole_register_name::BC); break;
            case 0x13: inc(registers::whole_register_name::DE); break;
            case 0x23: inc(registers::whole_register_name::HL); break;
            case 0x33: inc(registers::whole_register_name::SP); break;

            //dec 16bit
            case 0x0B: dec(registers::whole_register_name::BC); break;
            case 0x1B: dec(registers::whole_register_name::DE); break;
            case 0x2B: dec(registers::whole_register_name::HL); break;
            case 0x3B: dec(registers::whole_register_name::SP); break;
        }

        prefetch_next_instruction();
    }
}
