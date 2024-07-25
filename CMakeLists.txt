cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Aluminium-Store LANGUAGES CXX)

set(OUTPUT_DIR linux-x86_64-${CMAKE_BUILD_TYPE})

# Common stuff

set(AL_COMMON_PATH ${PROJECT_SOURCE_DIR}/Aluminium-Common)
set(AL_COMMON_SOURCE_PATH ${AL_COMMON_PATH}/src)
set(AL_COMMON_INCLUDE_PATH ${AL_COMMON_PATH}/include)
set(AL_COMMON_LIB_PATH ${AL_COMMON_PATH}/lib)

set(AL_COMMON_SOURCES ${AL_COMMON_SOURCE_PATH}/Log.cpp)

set(AL_COMMON_INCLUDES ${AL_COMMON_INCLUDE_PATH} ${AL_COMMON_LIB_PATH}/GameNetworkingSockets/include ${AL_COMMON_LIB_PATH}/fmt)
set(AL_COMMON_LIBRARIES ${AL_COMMON_LIB_PATH}/GameNetworkingSockets/bin/Linux/libGameNetworkingSockets.so)

add_subdirectory(Aluminium-Client)
add_subdirectory(Aluminium-Server)
