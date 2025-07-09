cmake_minimum_required(VERSION 3.26)
project(MemoryPoolAllocator)

set(CMAKE_CXX_STANDARD 23)

add_executable(MemoryPoolAllocator main.cpp
        memory_pool.cpp
        memory_pool.hpp)
