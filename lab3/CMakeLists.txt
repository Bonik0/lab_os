cmake_minimum_required(VERSION 3.10)
project(lab03)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main parent.cpp)
add_executable(child1 child1.cpp)
add_executable(child2 child2.cpp)