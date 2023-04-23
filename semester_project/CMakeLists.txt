cmake_minimum_required(VERSION 3.24)
project(semester_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(semester_project src/main.cpp src/cpu/central_processing_unit.cpp src/cpu/central_processing_unit.hpp src/cpu/registers.hpp src/utility.hpp src/memory.cpp src/cpu/registers.cpp src/cpu/cpu_execute_table.cpp src/cpu/cpu_execute_methods.cpp src/emulator.cpp src/hardware/ppu.cpp src/hardware/ppu.hpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)
