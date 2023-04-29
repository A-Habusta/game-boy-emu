// File: joypad.hpp
//
// Created by Adrian Habusta on 28.04.2023
//

#ifndef SEMESTER_PROJECT_JOYPAD_HPP
#define SEMESTER_PROJECT_JOYPAD_HPP

#include <SDL.h>

#include <utility>

#include <utility>

#include "../cpu/cpu_interrupt_typedef.hpp"
#include "../utility.hpp"

class joypad {
    enum {
        read_direction_buttons_pos = 4,
        read_action_buttons_pos = 5,

        joypad_source_mask = 0b00110000,

        default_keys_low_nibble = 0x0F
    };

    enum {
        key_right_pos = 0,
        key_left_pos = 1,
        key_up_pos = 2,
        key_down_pos = 3,

        key_a_pos = 0,
        key_b_pos = 1,
        key_select_pos = 2,
        key_start_pos = 3
    };

    byte joypad_status{0xC0 | default_keys_low_nibble};

    byte joypad_direction_keys_state{default_keys_low_nibble};
    byte joypad_action_keys_state{default_keys_low_nibble};

    interrupt_callback request_joystick_interrupt;

    bool check_for_keys_high_to_low_transition(byte new_status) const;

    // Inverted logic, pressed keys write 0, released keys 1
    void update_pressed_keys_from_event(SDL_Event& event) { update_keys_from_event(event, false); }
    void update_released_keys_from_event(SDL_Event& event) { update_keys_from_event(event, true); }

    void update_keys_from_event(SDL_Event& event, bool new_value);

    bool are_direction_keys_selected() const { return !utility::get_bit(joypad_status, read_direction_buttons_pos); }
    bool are_action_keys_selected() const { return !utility::get_bit(joypad_status, read_action_buttons_pos); }

    void update_joypad_status();

public:
    explicit joypad(interrupt_callback&& callback) : request_joystick_interrupt(std::move(callback)) {}

    void handle_input();

    void write_joypad_status(byte value);
    byte read_joypad_status() const;
};

#endif //SEMESTER_PROJECT_JOYPAD_HPP
