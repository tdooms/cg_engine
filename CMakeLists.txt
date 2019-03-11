cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_FLAGS "-O3 -Wextra -Wall")
set(CMAKE_CXX_STANDARD 14)

file(GLOB HDRS ${engine_SOURCE_DIR}/*.h ${engine_SOURCE_DIR}/math/*.h ${engine_SOURCE_DIR}/geometry/*.h)
file(GLOB SRCS ${engine_SOURCE_DIR}/*.cpp ${engine_SOURCE_DIR}/math/*.cpp ${engine_SOURCE_DIR}/geometry/*.cpp)

add_executable(engine ${HDRS} ${SRCS})