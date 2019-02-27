cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_FLAGS "-O3 -Wextra -Wall")
set(CMAKE_CXX_STANDARD 14)

add_executable(engine engine.cc easy_image.cc easy_image.h ini_configuration.cc ini_configuration.h l_parser.cc l_parser.h l_renderer.cpp l_renderer.h primitives.h primitives.cpp)