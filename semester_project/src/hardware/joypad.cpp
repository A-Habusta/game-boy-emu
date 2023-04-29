// File: joypad.cpp
//
// Created by Adrian Habusta on 28.04.2023
//

#include "../emulator.hpp"
#include "joypad.hpp"

void joypad::handle_input() {
    SDL_Event event;
    // PollEvent also updates the global keyboard state
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: throw emulator::exit();
            case SDL_KEYDOWN: update_pressed_keys_from_event(event); break;
            case SDL_KEYUP: update_released_keys_from_event(event); break;

            default: break;
        }
    }

    update_joypad_status();
}

// Keys are not remappable right now
void joypad::update_keys_from_event(SDL_Event &event, bool new_value) const {
    switch (event.key.keysym.sym) {
        case SDLK_UP: utility::write_bit(joypad_direction_keys_state, key_up_pos, new_value); break;
        case SDLK_DOWN: utility::write_bit(joypad_direction_keys_state, key_down_pos, new_value); break;
        case SDLK_LEFT: utility::write_bit(joypad_direction_keys_state, key_left_pos, new_value); break;
        case SDLK_RIGHT: utility::write_bit(joypad_direction_keys_state, key_right_pos, new_value); break;

        case SDLK_z: utility::write_bit(joypad_action_keys_state, key_a_pos, new_value); break;
        case SDLK_x: utility::write_bit(joypad_action_keys_state, key_b_pos, new_value); break;
        case SDLK_RETURN: utility::write_bit(joypad_action_keys_state, key_start_pos, new_value); break;
        case SDLK_SPACE: utility::write_bit(joypad_action_keys_state, key_select_pos, new_value); break;

        default: break;
    }
}

bool joypad::check_for_keys_high_to_low_transition(byte new_status) const {
    byte old_status_lower_nibble = utility::get_lower_nibble(joypad_status);
    byte new_status_lower_nibble = utility::get_lower_nibble(new_status);

    // This operation works because the upper nibble will always contain zeroes, and there will only be ones in places
    // where the old status was 1 and the new status is 0
    return (old_status_lower_nibble & ~new_status_lower_nibble) != 0;
}

void joypad::update_joypad_status() {
    byte result_upper_nibble = joypad_status & joypad_source_mask;
    byte result_lower_nibble = default_keys_low_nibble;

    // They can both be selected
    if (are_action_keys_selected())
        result_lower_nibble &= joypad_action_keys_state;

    if (are_direction_keys_selected())
        result_lower_nibble &= joypad_direction_keys_state;

    byte result = result_upper_nibble | result_lower_nibble;

    bool result_changed = check_for_keys_high_to_low_transition(result);

    if (result_changed)
        request_joystick_interrupt();

    joypad_status = result;
}


byte joypad::read_joypad_status() const {
    return joypad_status;
}

void joypad::write_joypad_status(byte value) {
    // We can only write to these two bits
    value &= joypad_source_mask;

    // Upper bits can stay, since they are always zero
    joypad_status &= ~joypad_source_mask;
    joypad_status |= value;

    update_joypad_status();
}