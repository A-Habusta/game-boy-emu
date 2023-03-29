cmake_minimum_required(VERSION 3.24)
project(semester_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(semester_project main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})
