cmake_minimum_required(VERSION 3.17)
project(os_lab_4 DESCRIPTION "laboratory work on course OS")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(static_lib STATIC lib/pi-sin-1.c)

add_executable(static static.c)

target_link_libraries(static PRIVATE static_lib)