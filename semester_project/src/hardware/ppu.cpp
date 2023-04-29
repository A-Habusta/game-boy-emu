// File: pixel_processing_unit.cpp
//
// Created by Adrian Habusta on 24.04.2023
//

#include "ppu.hpp"

namespace pixel_processing_unit {
    void ppu::run_machine_cycle() {
        if (!is_powered_on)
            return;

        for (int i = 0; i < t_cycles_per_m_cycle; i++)
            run_t_cycle();
    }

    void ppu::run_t_cycle() {
        if (remaining_t_cycles-- == 0) {
            move_to_next_mode();
        }

        switch (current_mode) {
            case mode::oam_search: run_oam_search_t_cycle(); break;
            case mode::pixel_transfer: run_pixel_transfer_t_cycle(); break;
            case mode::h_blank: run_h_blank_t_cycle(); break;
            case mode::v_blank: run_v_blank_t_cycle(); break;

            // Can't occur
            default: break;
        }
    }

    void ppu::run_oam_search_t_cycle() {
        // Only do something if first cycle
        if (remaining_t_cycles != t_cycles_per_oam_read)
            return;

        increment_line_counter_and_check_for_match();

        sprite::size sprite_size = registers.get_sprite_size();
        current_line_sprites = oam.create_sprite_cache_for_line(registers.lcd_y, sprite_size);
    }

    void ppu::run_pixel_transfer_t_cycle() {
        int current_x = t_cycles_per_pixel_transfer  - remaining_t_cycles;

        if (current_x >= screen_pixel_width)
            return;

        auto actual_pixel = get_pixel(current_x);
        renderer.save_pixel(current_x, registers.lcd_y, actual_pixel);
    }

    void ppu::run_h_blank_t_cycle() {
        // Nothing happens here
        return;
    }

    void ppu::run_v_blank_t_cycle() {
        // Render frame if first cycle
        if (remaining_t_cycles == t_cycles_per_v_blank) {
            renderer.render_frame();
        }

        if (remaining_t_cycles % t_cycles_per_scanline == 0) {
            increment_line_counter_and_check_for_match();
        }
    }

    palette::real_pixel_type ppu::get_pixel(int x) {
        palette::pixel bg_pixel{0};

        bool draw_bg_elements = registers.get_bg_window_display_priority();
        if (draw_bg_elements) {
            if (check_if_in_window(x) && registers.get_window_draw_enable())
                bg_pixel = get_pixel_from_window_layer(x);
            else
                bg_pixel = get_pixel_from_background_layer(x);
        }

        if (!registers.get_sprite_draw_enable()) {
            return registers.background_palette.convert_to_real_pixel(bg_pixel);
        }

        std::optional<sprite> current_sprite = current_line_sprites->get_first_sprite_at_current_x(x);
        if (!current_sprite) {
            // Return bg if there is no sprite
            return registers.background_palette.convert_to_real_pixel(bg_pixel);
        }
        auto sprite_size = registers.get_sprite_size();

        palette::pixel sprite_pixel = get_pixel_from_sprite(x, *current_sprite, sprite_size);

        while (sprite_pixel.is_transparent() ) {
            current_sprite = current_line_sprites->get_next_sprite_at_current_x(x);
            if (!current_sprite) {
                // Return bg if there is no non-transparent sprite
                return registers.background_palette.convert_to_real_pixel(bg_pixel);
            }
            sprite_pixel = get_pixel_from_sprite(x, *current_sprite, sprite_size);
        }

        if (current_sprite->get_priority() && !bg_pixel.is_transparent()) {
            // Return bg if "bg over sprite" priority is set and bg is not transparent
            return registers.background_palette.convert_to_real_pixel(bg_pixel);
        }

        // Return sprite otherwise
        bool palette_index = current_sprite->get_palette_number();
        palette sprite_palette = palette_index ? registers.sprite_palette_1 : registers.sprite_palette_0;

        return sprite_palette.convert_to_real_pixel(sprite_pixel);

    }

    palette::pixel ppu::get_pixel_from_sprite(int x, sprite current_sprite, sprite::size sprite_size) {
        int sprite_width = sprite::width;
        int sprite_height = sprite::get_height_from_size(sprite_size);

        int sprite_x = x - current_sprite.get_corrected_x();
        int sprite_y = registers.lcd_y - current_sprite.get_corrected_y();


        if (current_sprite.get_x_flip())
            sprite_x = sprite_width - sprite_x - 1;
        if (current_sprite.get_y_flip())
            sprite_y = sprite_height - sprite_y - 1;

        auto tile_number = current_sprite.get_tile_number();
        auto tile = vram.tiles.tiles.get_tile_oam(tile_number);

        return tile.get_pixel(sprite_x, sprite_y);
    }

    palette::pixel ppu::get_pixel_from_background_layer(int x) const {
        int bg_x = (x + registers.scroll_x) & 0xFF;
        int bg_y = (registers.lcd_y + registers.scroll_y) & 0xFF;

        bool map_choice = registers.get_bg_tile_map_select();
        const tile_data::map& tile_map = map_choice ? vram.tiles.tile_map_1 : vram.tiles.tile_map_0;

        int tile_number_x = bg_x / tile::size;
        int tile_number_y = bg_y / tile::size;

        auto tile_index = tile_map.get_tile_index(tile_number_x, tile_number_y);

        bool method = registers.get_bg_and_window_tile_data_select();
        auto tile = vram.tiles.tiles.get_tile_bg_and_window(tile_index, method);

        int tile_x = bg_x % tile::size;
        int tile_y = bg_y % tile::size;

        return tile.get_pixel(tile_x, tile_y);
    }

    palette::pixel ppu::get_pixel_from_window_layer(int x) const {
        int window_x = x - (registers.window_x - 7);
        int window_y = registers.lcd_y - registers.window_y;

        bool map_choice = registers.get_window_tile_map_select();
        const tile_data::map& tile_map = map_choice ? vram.tiles.tile_map_1 : vram.tiles.tile_map_0;

        int tile_number_x = window_x / tile::size;
        int tile_number_y = window_y / tile::size;

        auto tile_index = tile_map.get_tile_index(tile_number_x, tile_number_y);

        bool method = registers.get_bg_and_window_tile_data_select();
        auto tile = vram.tiles.tiles.get_tile_bg_and_window(tile_index, method);

        int tile_x = window_x % tile::size;
        int tile_y = window_y % tile::size;

        return tile.get_pixel(tile_x, tile_y);
    }

    bool ppu::check_if_in_window(int x) const {
        return x >= registers.window_x - 7 && registers.lcd_y >= registers.window_y;
    }


    void ppu::move_to_next_mode() {
        mode next_mode = mode::oam_search;
        switch (current_mode) {
            case mode::oam_search:
                next_mode = mode::pixel_transfer;
                break;
            case mode::pixel_transfer:
                next_mode = mode::h_blank;
                break;
            case mode::h_blank:
                next_mode = registers.lcd_y < scanlines_per_screen - 1 ? mode::oam_search : mode::v_blank;
                break;
            case mode::v_blank:
                next_mode = mode::oam_search;
                break;

                // Can't occur
            default: break;
        }
        change_mode_to(next_mode);
    }

    void ppu::change_mode_to(mode new_mode) {
        current_mode = new_mode;
        remaining_t_cycles = mode_length[new_mode];
        registers.write_status_mode(new_mode);

        request_mode_change_interrupt(new_mode);
    }

    void ppu::request_mode_change_interrupt(mode new_mode) {
        switch (new_mode) {

            case mode::v_blank:
                // always request vblank interrupt
                request_v_blank_interrupt();
                if (registers.get_v_blank_interrupt_enable())
                    request_stat_interrupt();
                break;
            case mode::oam_search:
                if (registers.get_oam_interrupt_enable())
                    request_stat_interrupt();
                break;
            case mode::h_blank:
                if (registers.get_h_blank_interrupt_enable())
                    request_stat_interrupt();
                break;

            default: break;
        }
    }

    void ppu::increment_line_counter_and_check_for_match() {
        registers.lcd_y++;
        registers.lcd_y %= scanlines_per_frame;

        bool coincidence = registers.lcd_y == registers.lcd_y_compare;
        registers.write_coincidence_flag(coincidence);

        if (coincidence && registers.get_coincidence_interrupt_enable() ) {
            request_stat_interrupt();
        }
    }
}