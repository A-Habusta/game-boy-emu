// File: cpu_execute_table.cpp
//
// Created by Adrian Habusta on 19.04.2023
//

#include "central_processing_unit.hpp"
#include "../utility.hpp"

namespace central_processing_unit {
    void cpu::execute_instruction(byte instruction) {
        // I am hoping that this is compiled into a jump table instead of an if-else cascade, since this is a pretty
        // simple and readable way of doing this.
        switch(instruction) {
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

            //load HL <- SP + immediate
            case 0xF8: load(registers::whole_register_name::HL, add_to_sp(read_byte_at_pc_with_increment())); break;

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
            case 0xE8: registers.whole.SP = add_to_sp(read_byte_at_pc_with_increment()); run_phantom_cycle(); break;

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
            case 0x9D: sub(registers.half.L, registers.read_carry_flag()); break;
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

            //carry
            case 0x37: set_carry_flag(1); break;
            case 0x3F: set_carry_flag(!registers.read_carry_flag()); break;

            //complement A
            case 0x2F: complement_A(); break;
            //A -> binary coded decimal
            case 0x27: daa(); break;

            //control flow
            case 0xC3: jump(read_word_at_pc_with_increment(), true); break;
            case 0xC2: jump(read_word_at_pc_with_increment(), !registers.read_zero_flag()); break;
            case 0xCA: jump(read_word_at_pc_with_increment(), registers.read_zero_flag()); break;
            case 0xD2: jump(read_word_at_pc_with_increment(), !registers.read_carry_flag()); break;
            case 0xDA: jump(read_word_at_pc_with_increment(), registers.read_carry_flag()); break;

            case 0xE9: jump_hl(); break;

            case 0x18: jump_relative(read_byte_at_pc_with_increment(), true); break;
            case 0x20: jump_relative(read_byte_at_pc_with_increment(), !registers.read_zero_flag()); break;
            case 0x28: jump_relative(read_byte_at_pc_with_increment(), registers.read_zero_flag()); break;
            case 0x30: jump_relative(read_byte_at_pc_with_increment(), !registers.read_carry_flag()); break;
            case 0x38: jump_relative(read_byte_at_pc_with_increment(), registers.read_carry_flag()); break;

            case 0xCD: call(read_word_at_pc_with_increment(), true); break;
            case 0xC4: call(read_word_at_pc_with_increment(), !registers.read_zero_flag()); break;
            case 0xCC: call(read_word_at_pc_with_increment(), registers.read_zero_flag()); break;
            case 0xD4: call(read_word_at_pc_with_increment(), !registers.read_carry_flag()); break;
            case 0xDC: call(read_word_at_pc_with_increment(), registers.read_carry_flag()); break;

            case 0xC9: return_always(); break;
            case 0xC0: return_conditional(!registers.read_zero_flag()); break;
            case 0xC8: return_conditional(registers.read_zero_flag()); break;
            case 0xD0: return_conditional(!registers.read_carry_flag()); break;
            case 0xD8: return_conditional(registers.read_carry_flag()); break;

            case 0xD9: return_always(), interrupt_master_enable = true; break;

            case 0xC7: call(0x00, true); break;
            case 0xCF: call(0x08, true); break;
            case 0xD7: call(0x10, true); break;
            case 0xDF: call(0x18, true); break;
            case 0xE7: call(0x20, true); break;
            case 0xEF: call(0x28, true); break;
            case 0xF7: call(0x30, true); break;
            case 0xFF: call(0x38, true); break;

            //misc
            case 0x00: break;
            case 0x10: stop(); break;
            case 0x76: halt(); break;
            case 0xCB: execute_cb_prefixed_instruction(); break;
            case 0xF3: disable_interrupts(); break;
            case 0xFB: queue_enable_interrupts(); break;

            //bitwise shifts

            //rlca
            case 0x07: rotate_a_left(utility::get_bit(registers.half.A, 7)); break;
            //rla
            case 0x17: rotate_a_left(registers.read_carry_flag()); break;
            //rrca
            case 0x0F: rotate_a_right(utility::get_bit(registers.half.A, 0)); break;
            //rra
            case 0x1F: rotate_a_right(registers.read_carry_flag()); break;

            // unknown
            default: handle_unknown_instruction(); break;
        }
    }

    void cpu::execute_cb_prefixed_instruction() {
        byte instruction = read_byte_at_pc_with_increment();
        // FIXME: Should be changed into an explicit table, since there is a clear pattern
        switch(instruction) {
            //rlc
            case 0x07: rlc(registers::half_register_name::A); break;
            case 0x00: rlc(registers::half_register_name::B); break;
            case 0x01: rlc(registers::half_register_name::C); break;
            case 0x02: rlc(registers::half_register_name::D); break;
            case 0x03: rlc(registers::half_register_name::E); break;
            case 0x04: rlc(registers::half_register_name::H); break;
            case 0x05: rlc(registers::half_register_name::L); break;
            case 0x06: rlc(registers.whole.HL); break;

            //rl
            case 0x17: rl(registers::half_register_name::A); break;
            case 0x10: rl(registers::half_register_name::B); break;
            case 0x11: rl(registers::half_register_name::C); break;
            case 0x12: rl(registers::half_register_name::D); break;
            case 0x13: rl(registers::half_register_name::E); break;
            case 0x14: rl(registers::half_register_name::H); break;
            case 0x15: rl(registers::half_register_name::L); break;
            case 0x16: rl(registers.whole.HL); break;

            //rrc
            case 0x0F: rrc(registers::half_register_name::A); break;
            case 0x08: rrc(registers::half_register_name::B); break;
            case 0x09: rrc(registers::half_register_name::C); break;
            case 0x0A: rrc(registers::half_register_name::D); break;
            case 0x0B: rrc(registers::half_register_name::E); break;
            case 0x0C: rrc(registers::half_register_name::H); break;
            case 0x0D: rrc(registers::half_register_name::L); break;
            case 0x0E: rrc(registers.whole.HL); break;

            //rr
            case 0x1F: rr(registers::half_register_name::A); break;
            case 0x18: rr(registers::half_register_name::B); break;
            case 0x19: rr(registers::half_register_name::C); break;
            case 0x1A: rr(registers::half_register_name::D); break;
            case 0x1B: rr(registers::half_register_name::E); break;
            case 0x1C: rr(registers::half_register_name::H); break;
            case 0x1D: rr(registers::half_register_name::L); break;
            case 0x1E: rr(registers.whole.HL); break;

            //sla
            case 0x27: sla(registers::half_register_name::A); break;
            case 0x20: sla(registers::half_register_name::B); break;
            case 0x21: sla(registers::half_register_name::C); break;
            case 0x22: sla(registers::half_register_name::D); break;
            case 0x23: sla(registers::half_register_name::E); break;
            case 0x24: sla(registers::half_register_name::H); break;
            case 0x25: sla(registers::half_register_name::L); break;
            case 0x26: sla(registers.whole.HL); break;

            //sra
            case 0x2F: sra(registers::half_register_name::A); break;
            case 0x28: sra(registers::half_register_name::B); break;
            case 0x29: sra(registers::half_register_name::C); break;
            case 0x2A: sra(registers::half_register_name::D); break;
            case 0x2B: sra(registers::half_register_name::E); break;
            case 0x2C: sra(registers::half_register_name::H); break;
            case 0x2D: sra(registers::half_register_name::L); break;
            case 0x2E: sra(registers.whole.HL); break;

            //srl
            case 0x3F: srl(registers::half_register_name::A); break;
            case 0x38: srl(registers::half_register_name::B); break;
            case 0x39: srl(registers::half_register_name::C); break;
            case 0x3A: srl(registers::half_register_name::D); break;
            case 0x3B: srl(registers::half_register_name::E); break;
            case 0x3C: srl(registers::half_register_name::H); break;
            case 0x3D: srl(registers::half_register_name::L); break;
            case 0x3E: srl(registers.whole.HL); break;

            //swap
            case 0x37: swap(registers::half_register_name::A); break;
            case 0x30: swap(registers::half_register_name::B); break;
            case 0x31: swap(registers::half_register_name::C); break;
            case 0x32: swap(registers::half_register_name::D); break;
            case 0x33: swap(registers::half_register_name::E); break;
            case 0x34: swap(registers::half_register_name::H); break;
            case 0x35: swap(registers::half_register_name::L); break;
            case 0x36: swap(registers.whole.HL); break;

            //bit
            case 0x47: bit(0, registers::half_register_name::A); break;
            case 0x40: bit(0, registers::half_register_name::B); break;
            case 0x41: bit(0, registers::half_register_name::C); break;
            case 0x42: bit(0, registers::half_register_name::D); break;
            case 0x43: bit(0, registers::half_register_name::E); break;
            case 0x44: bit(0, registers::half_register_name::H); break;
            case 0x45: bit(0, registers::half_register_name::L); break;
            case 0x46: bit(0, registers.whole.HL); break;

            case 0x4F: bit(1, registers::half_register_name::A); break;
            case 0x48: bit(1, registers::half_register_name::B); break;
            case 0x49: bit(1, registers::half_register_name::C); break;
            case 0x4A: bit(1, registers::half_register_name::D); break;
            case 0x4B: bit(1, registers::half_register_name::E); break;
            case 0x4C: bit(1, registers::half_register_name::H); break;
            case 0x4D: bit(1, registers::half_register_name::L); break;
            case 0x4E: bit(1, registers.whole.HL); break;

            case 0x57: bit(2, registers::half_register_name::A); break;
            case 0x50: bit(2, registers::half_register_name::B); break;
            case 0x51: bit(2, registers::half_register_name::C); break;
            case 0x52: bit(2, registers::half_register_name::D); break;
            case 0x53: bit(2, registers::half_register_name::E); break;
            case 0x54: bit(2, registers::half_register_name::H); break;
            case 0x55: bit(2, registers::half_register_name::L); break;
            case 0x56: bit(2, registers.whole.HL); break;

            case 0x5F: bit(3, registers::half_register_name::A); break;
            case 0x58: bit(3, registers::half_register_name::B); break;
            case 0x59: bit(3, registers::half_register_name::C); break;
            case 0x5A: bit(3, registers::half_register_name::D); break;
            case 0x5B: bit(3, registers::half_register_name::E); break;
            case 0x5C: bit(3, registers::half_register_name::H); break;
            case 0x5D: bit(3, registers::half_register_name::L); break;
            case 0x5E: bit(3, registers.whole.HL); break;

            case 0x67: bit(4, registers::half_register_name::A); break;
            case 0x60: bit(4, registers::half_register_name::B); break;
            case 0x61: bit(4, registers::half_register_name::C); break;
            case 0x62: bit(4, registers::half_register_name::D); break;
            case 0x63: bit(4, registers::half_register_name::E); break;
            case 0x64: bit(4, registers::half_register_name::H); break;
            case 0x65: bit(4, registers::half_register_name::L); break;
            case 0x66: bit(4, registers.whole.HL); break;

            case 0x6F: bit(5, registers::half_register_name::A); break;
            case 0x68: bit(5, registers::half_register_name::B); break;
            case 0x69: bit(5, registers::half_register_name::C); break;
            case 0x6A: bit(5, registers::half_register_name::D); break;
            case 0x6B: bit(5, registers::half_register_name::E); break;
            case 0x6C: bit(5, registers::half_register_name::H); break;
            case 0x6D: bit(5, registers::half_register_name::L); break;
            case 0x6E: bit(5, registers.whole.HL); break;

            case 0x77: bit(6, registers::half_register_name::A); break;
            case 0x70: bit(6, registers::half_register_name::B); break;
            case 0x71: bit(6, registers::half_register_name::C); break;
            case 0x72: bit(6, registers::half_register_name::D); break;
            case 0x73: bit(6, registers::half_register_name::E); break;
            case 0x74: bit(6, registers::half_register_name::H); break;
            case 0x75: bit(6, registers::half_register_name::L); break;
            case 0x76: bit(6, registers.whole.HL); break;

            case 0x7F: bit(7, registers::half_register_name::A); break;
            case 0x78: bit(7, registers::half_register_name::B); break;
            case 0x79: bit(7, registers::half_register_name::C); break;
            case 0x7A: bit(7, registers::half_register_name::D); break;
            case 0x7B: bit(7, registers::half_register_name::E); break;
            case 0x7C: bit(7, registers::half_register_name::H); break;
            case 0x7D: bit(7, registers::half_register_name::L); break;
            case 0x7E: bit(7, registers.whole.HL); break;

            //res
            case 0x87: res(0, registers::half_register_name::A); break;
            case 0x80: res(0, registers::half_register_name::B); break;
            case 0x81: res(0, registers::half_register_name::C); break;
            case 0x82: res(0, registers::half_register_name::D); break;
            case 0x83: res(0, registers::half_register_name::E); break;
            case 0x84: res(0, registers::half_register_name::H); break;
            case 0x85: res(0, registers::half_register_name::L); break;
            case 0x86: res(0, registers.whole.HL); break;

            case 0x8F: res(1, registers::half_register_name::A); break;
            case 0x88: res(1, registers::half_register_name::B); break;
            case 0x89: res(1, registers::half_register_name::C); break;
            case 0x8A: res(1, registers::half_register_name::D); break;
            case 0x8B: res(1, registers::half_register_name::E); break;
            case 0x8C: res(1, registers::half_register_name::H); break;
            case 0x8D: res(1, registers::half_register_name::L); break;
            case 0x8E: res(1, registers.whole.HL); break;

            case 0x97: res(2, registers::half_register_name::A); break;
            case 0x90: res(2, registers::half_register_name::B); break;
            case 0x91: res(2, registers::half_register_name::C); break;
            case 0x92: res(2, registers::half_register_name::D); break;
            case 0x93: res(2, registers::half_register_name::E); break;
            case 0x94: res(2, registers::half_register_name::H); break;
            case 0x95: res(2, registers::half_register_name::L); break;
            case 0x96: res(2, registers.whole.HL); break;

            case 0x9F: res(3, registers::half_register_name::A); break;
            case 0x98: res(3, registers::half_register_name::B); break;
            case 0x99: res(3, registers::half_register_name::C); break;
            case 0x9A: res(3, registers::half_register_name::D); break;
            case 0x9B: res(3, registers::half_register_name::E); break;
            case 0x9C: res(3, registers::half_register_name::H); break;
            case 0x9D: res(3, registers::half_register_name::L); break;
            case 0x9E: res(3, registers.whole.HL); break;

            case 0xA7: res(4, registers::half_register_name::A); break;
            case 0xA0: res(4, registers::half_register_name::B); break;
            case 0xA1: res(4, registers::half_register_name::C); break;
            case 0xA2: res(4, registers::half_register_name::D); break;
            case 0xA3: res(4, registers::half_register_name::E); break;
            case 0xA4: res(4, registers::half_register_name::H); break;
            case 0xA5: res(4, registers::half_register_name::L); break;
            case 0xA6: res(4, registers.whole.HL); break;

            case 0xAF: res(5, registers::half_register_name::A); break;
            case 0xA8: res(5, registers::half_register_name::B); break;
            case 0xA9: res(5, registers::half_register_name::C); break;
            case 0xAA: res(5, registers::half_register_name::D); break;
            case 0xAB: res(5, registers::half_register_name::E); break;
            case 0xAC: res(5, registers::half_register_name::H); break;
            case 0xAD: res(5, registers::half_register_name::L); break;
            case 0xAE: res(5, registers.whole.HL); break;

            case 0xB7: res(6, registers::half_register_name::A); break;
            case 0xB0: res(6, registers::half_register_name::B); break;
            case 0xB1: res(6, registers::half_register_name::C); break;
            case 0xB2: res(6, registers::half_register_name::D); break;
            case 0xB3: res(6, registers::half_register_name::E); break;
            case 0xB4: res(6, registers::half_register_name::H); break;
            case 0xB5: res(6, registers::half_register_name::L); break;
            case 0xB6: res(6, registers.whole.HL); break;

            case 0xBF: res(7, registers::half_register_name::A); break;
            case 0xB8: res(7, registers::half_register_name::B); break;
            case 0xB9: res(7, registers::half_register_name::C); break;
            case 0xBA: res(7, registers::half_register_name::D); break;
            case 0xBB: res(7, registers::half_register_name::E); break;
            case 0xBC: res(7, registers::half_register_name::H); break;
            case 0xBD: res(7, registers::half_register_name::L); break;
            case 0xBE: res(7, registers.whole.HL); break;

            // set
            case 0xC7: set(0, registers::half_register_name::A); break;
            case 0xC0: set(0, registers::half_register_name::B); break;
            case 0xC1: set(0, registers::half_register_name::C); break;
            case 0xC2: set(0, registers::half_register_name::D); break;
            case 0xC3: set(0, registers::half_register_name::E); break;
            case 0xC4: set(0, registers::half_register_name::H); break;
            case 0xC5: set(0, registers::half_register_name::L); break;
            case 0xC6: set(0, registers.whole.HL); break;

            case 0xCF: set(1, registers::half_register_name::A); break;
            case 0xC8: set(1, registers::half_register_name::B); break;
            case 0xC9: set(1, registers::half_register_name::C); break;
            case 0xCA: set(1, registers::half_register_name::D); break;
            case 0xCB: set(1, registers::half_register_name::E); break;
            case 0xCC: set(1, registers::half_register_name::H); break;
            case 0xCD: set(1, registers::half_register_name::L); break;
            case 0xCE: set(1, registers.whole.HL); break;

            case 0xD7: set(2, registers::half_register_name::A); break;
            case 0xD0: set(2, registers::half_register_name::B); break;
            case 0xD1: set(2, registers::half_register_name::C); break;
            case 0xD2: set(2, registers::half_register_name::D); break;
            case 0xD3: set(2, registers::half_register_name::E); break;
            case 0xD4: set(2, registers::half_register_name::H); break;
            case 0xD5: set(2, registers::half_register_name::L); break;
            case 0xD6: set(2, registers.whole.HL); break;

            case 0xDF: set(3, registers::half_register_name::A); break;
            case 0xD8: set(3, registers::half_register_name::B); break;
            case 0xD9: set(3, registers::half_register_name::C); break;
            case 0xDA: set(3, registers::half_register_name::D); break;
            case 0xDB: set(3, registers::half_register_name::E); break;
            case 0xDC: set(3, registers::half_register_name::H); break;
            case 0xDD: set(3, registers::half_register_name::L); break;
            case 0xDE: set(3, registers.whole.HL); break;

            case 0xE7: set(4, registers::half_register_name::A); break;
            case 0xE0: set(4, registers::half_register_name::B); break;
            case 0xE1: set(4, registers::half_register_name::C); break;
            case 0xE2: set(4, registers::half_register_name::D); break;
            case 0xE3: set(4, registers::half_register_name::E); break;
            case 0xE4: set(4, registers::half_register_name::H); break;
            case 0xE5: set(4, registers::half_register_name::L); break;
            case 0xE6: set(4, registers.whole.HL); break;

            case 0xEF: set(5, registers::half_register_name::A); break;
            case 0xE8: set(5, registers::half_register_name::B); break;
            case 0xE9: set(5, registers::half_register_name::C); break;
            case 0xEA: set(5, registers::half_register_name::D); break;
            case 0xEB: set(5, registers::half_register_name::E); break;
            case 0xEC: set(5, registers::half_register_name::H); break;
            case 0xED: set(5, registers::half_register_name::L); break;
            case 0xEE: set(5, registers.whole.HL); break;

            case 0xF7: set(6, registers::half_register_name::A); break;
            case 0xF0: set(6, registers::half_register_name::B); break;
            case 0xF1: set(6, registers::half_register_name::C); break;
            case 0xF2: set(6, registers::half_register_name::D); break;
            case 0xF3: set(6, registers::half_register_name::E); break;
            case 0xF4: set(6, registers::half_register_name::H); break;
            case 0xF5: set(6, registers::half_register_name::L); break;
            case 0xF6: set(6, registers.whole.HL); break;

            case 0xFF: set(7, registers::half_register_name::A); break;
            case 0xF8: set(7, registers::half_register_name::B); break;
            case 0xF9: set(7, registers::half_register_name::C); break;
            case 0xFA: set(7, registers::half_register_name::D); break;
            case 0xFB: set(7, registers::half_register_name::E); break;
            case 0xFC: set(7, registers::half_register_name::H); break;
            case 0xFD: set(7, registers::half_register_name::L); break;
            case 0xFE: set(7, registers.whole.HL); break;

            // Will never occur since every possible byte is exhausted
            default: break;
        }
    }
}
