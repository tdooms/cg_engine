cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_FLAGS "-Wextra -Wall")
set(CMAKE_CXX_STANDARD 14)

file(GLOB HDRS ${engine_SOURCE_DIR}/*.h ${engine_SOURCE_DIR}/math/*.h)
file(GLOB SRCS ${engine_SOURCE_DIR}/*.cpp ${engine_SOURCE_DIR}/*.cc ${engine_SOURCE_DIR}/math/*.cpp)

add_executable(engine ${HDRS} ${SRCS})