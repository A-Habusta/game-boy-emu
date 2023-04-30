cmake_minimum_required(VERSION 3.24)
project(semester_project)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -O3)

add_executable(semester_project src/main.cpp src/cpu/central_processing_unit.cpp src/cpu/central_processing_unit.hpp src/cpu/registers.hpp src/utility.hpp src/emulator.cpp src/cpu/registers.cpp src/cpu/cpu_execute_table.cpp src/cpu/cpu_execute_methods.cpp src/hardware/ppu.cpp src/hardware/ppu.hpp src/hardware/ppu_data.hpp src/hardware/apu.cpp src/hardware/apu.hpp src/hardware/timer.cpp src/hardware/timer.hpp src/cpu/cpu_interrupt_typedef.hpp src/hardware/cartridge.cpp src/hardware/cartridge.hpp src/hardware/ram.hpp src/emulator_io_memory_map.cpp src/hardware/joypad.hpp src/hardware/joypad.cpp src/hardware/cartridge_memory_controllers.cpp src/hardware/cartridge_memory_controllers.hpp)

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARIES})