// File: pixel_processing_unit.hpp
//
// Created by Adrian Habusta on 22.04.2023
//

#ifndef SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP
#define SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP

#include "../utility.hpp"

namespace pixel_processing_unit {
    constexpr int SCREEN_WIDTH = 160;
    constexpr int SCREEN_HEIGHT = 144;
    constexpr int SCREEN_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

    constexpr int TILE_WIDTH = 8;
    constexpr int TILE_HEIGHT = 8;
    constexpr int TILE_BYTES_PER_LINE = 2;
    constexpr int BYTES_PER_TILE = TILE_HEIGHT * TILE_BYTES_PER_LINE;

    constexpr int SPRITE_COUNT = 40;
    constexpr int MAX_SPRITES_PER_LINE = 10;

    constexpr int VRAM_TILE_AREA_START = 0x0000;
    constexpr int VRAM_TILE_AREA_END = 0x17FF;
    constexpr int VRAM_TILE_AREA_SIZE = VRAM_TILE_AREA_END - VRAM_TILE_AREA_START + 1;
    constexpr int VRAM_TILE_COUNT = VRAM_TILE_AREA_SIZE / BYTES_PER_TILE;

    constexpr int VRAM_TILE_MAP_0_START = 0x1800;
    constexpr int VRAM_TILE_MAP_0_END = 0x1BFF;
    constexpr int VRAM_TILE_MAP_0_SIZE = VRAM_TILE_MAP_0_END - VRAM_TILE_MAP_0_START + 1;

    constexpr int VRAM_TILE_MAP_1_START = 0x1C00;
    constexpr int VRAM_TILE_MAP_1_END = 0x1FFF;
    constexpr int VRAM_TILE_MAP_1_SIZE = VRAM_TILE_MAP_1_END - VRAM_TILE_MAP_1_START + 1;

    constexpr int VRAM_SIZE = VRAM_TILE_AREA_SIZE + VRAM_TILE_MAP_0_SIZE + VRAM_TILE_MAP_1_SIZE;


    struct tile {
        byte get_pixel_color(int x, int y) const {
            byte color = 0;
            color |= utility::get_bit(data[y * TILE_BYTES_PER_LINE], 7 - x);
            color |= utility::get_bit(data[y * TILE_BYTES_PER_LINE + 1], 7 - x) << 1;
            return color;
        }
    private:
        byte data[BYTES_PER_TILE]{};
    };

    struct sprite {
        byte y{};
        byte x{};
        byte tile{};
        byte attributes{};

        enum class size {
            size8x8,
            size8x16
        };

        bool get_priority_flag() { return utility::get_bit(attributes, PRIORITY_FLAG_POSITION); }
        bool get_y_flip_flag() { return utility::get_bit(attributes, Y_FLIP_FLAG_POSITION); }
        bool get_x_flip_flag() { return utility::get_bit(attributes, X_FLIP_FLAG_POSITION); }
        byte get_palette_number() { return utility::get_bit(attributes, PALETTE_NUMBER_POSITION); }

    private:
        static constexpr int PRIORITY_FLAG_POSITION = 7;
        static constexpr int Y_FLIP_FLAG_POSITION = 6;
        static constexpr int X_FLIP_FLAG_POSITION = 5;
        static constexpr int PALETTE_NUMBER_POSITION = 4;
    };

    class ppu {
        enum class mode {
            H_BLANK = 0,
            V_BLANK = 1,
            OAM_SEARCH = 2,
            PIXEL_TRANSFER = 3
        };

        struct registers {
            byte lcd_control{};
            byte lcd_status{};
            byte scroll_y{};
            byte scroll_x{};
            byte line{};
            byte line_compare{};
            byte dma_transfer{};
            byte bg_palette{};
            byte obj_palette_0{};
            byte obj_palette_1{};
            byte window_y{};
            byte window_x{};
        } registers{};

        union oam {
            sprite sprites[SPRITE_COUNT]{};
            byte bytes[SPRITE_COUNT * sizeof(sprite)];
        } oam{};

        union vram {
            struct vram_tile_view {
                tile tiles[VRAM_TILE_COUNT];
                byte tile_map_0[VRAM_TILE_MAP_0_SIZE];
                byte tile_map_1[VRAM_TILE_MAP_1_SIZE];
            } tile_view{};
            byte bytes[VRAM_SIZE];
        } vram{};

        byte screen_buffer[SCREEN_SIZE]{};

    public:
        byte read_vram(word address);
        void write_vram(word address, byte value);

        byte read_oam(word address);
        void write_oam(word address, byte value);

        void run_machine_cycle();
    };
}

#endif //SEMESTER_PROJECT_PIXEL_PROCESSING_UNIT_HPP
