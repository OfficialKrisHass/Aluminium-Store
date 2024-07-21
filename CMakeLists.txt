cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Aluminium-Store LANGUAGES CXX)

set(OUTPUT_DIR linux-x86_64-${CMAKE_BUILD_TYPE})

add_subdirectory(Aluminium-Client)
add_subdirectory(Aluminium-Server)
