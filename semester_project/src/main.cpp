#include <SDL.h>
#include <iostream>

#include "cpu/central_processing_unit.hpp"
#include "hardware/pixel_processing_unit.hpp"

int main() {
    std::cout << sizeof(pixel_processing_unit::pixel) << ' ' << SDL_GetPixelFormatName(SDL_PIXELFORMAT_ARGB32) <<
    std::endl;
    return 0;
}