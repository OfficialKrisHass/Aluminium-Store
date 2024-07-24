message("------ Configuring Aluminium-Client ------")
message("")

# defines

set(AL_DEFINES AL_LINUX AL_CMAKE _CRT_SECURE_NO_WARNINGS)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(AL_DEFINES ${AL_DEFINES} AL_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(AL_DEFINES ${AL_DEFINES} AL_RELEASE)
endif()

# Final

add_executable(Aluminium-Client src/main.cpp src/Window.cpp src/UI.cpp src/User.cpp src/Network.cpp)
set_target_properties(Aluminium-Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${OUTPUT_DIR}/Aluminium-Client")

target_compile_definitions(Aluminium-Client PRIVATE ${AL_DEFINES})
target_include_directories(Aluminium-Client PRIVATE lib/GLFW/include lib/ImGui ${PROJECT_SOURCE_DIR}/lib/GameNetworkingSockets/include)

add_subdirectory(lib/GLFW)
add_subdirectory(lib/ImGui)

target_link_libraries(Aluminium-Client PRIVATE glfw ImGui ${PROJECT_SOURCE_DIR}/lib/GameNetworkingSockets/bin/Linux/libGameNetworkingSockets.so)
