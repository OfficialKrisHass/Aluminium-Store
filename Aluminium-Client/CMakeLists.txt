message("------ Configuring Aluminium-Client ------")
message("")

# defines

set(AL_DEFINES ${AL_COMMON_DEFINES})
set(AL_DEFINES ${AL_DEFINES} AL_CLIENT)

if(${CLIENT_LOGGING} STREQUAL ON OR ${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(AL_DEFINES ${AL_DEFINES} AL_LOGGING)
endif()

# sources

set(AL_SOURCES ${AL_COMMON_SOURCES})
set(AL_SOURCES ${AL_SOURCES} src/main.cpp src/Window.cpp src/UI.cpp src/User.cpp src/Network.cpp)

# includes

set(AL_INCLUDES ${AL_COMMON_INCLUDES})
set(AL_INCLUDES ${AL_INCLUDES} lib/GLFW/include lib/ImGui)

# Final

add_executable(Aluminium-Client ${AL_SOURCES})
set_target_properties(Aluminium-Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${OUTPUT_DIR}/Aluminium-Client")

target_compile_definitions(Aluminium-Client PRIVATE ${AL_DEFINES})
target_include_directories(Aluminium-Client PRIVATE ${AL_INCLUDES})

add_subdirectory(lib/GLFW)
add_subdirectory(lib/ImGui)

target_link_libraries(Aluminium-Client PRIVATE ${AL_COMMON_LIBRARIES} glfw ImGui)
