#include <SDL.h>
#include <iostream>

#include "cpu/central_processing_unit.hpp"
#include "hardware/ppu.hpp"

int main() {
    std::cout << ' ' << SDL_GetPixelFormatName(SDL_PIXELFORMAT_ARGB32) <<
    std::endl;
    return 0;
}