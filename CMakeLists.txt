cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_FLAGS "-O3 -Wextra -Wall")
set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE HDRS ${engine_SOURCE_DIR}/src/*.h  )
file(GLOB_RECURSE SRCS ${engine_SOURCE_DIR}/src/*.cpp)

add_executable(engine ${HDRS} ${SRCS})