message("------ Configuring Aluminium-Server ------")
message("")

add_executable(Aluminium-Server src/main.cpp src/Network.cpp)
set_target_properties(Aluminium-Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${OUTPUT_DIR}/Aluminium-Server")

target_include_directories(Aluminium-Server PRIVATE ${PROJECT_SOURCE_DIR}/lib/GameNetworkingSockets/include)

target_link_libraries(Aluminium-Server PRIVATE ${PROJECT_SOURCE_DIR}/lib/GameNetworkingSockets/bin/Linux/libGameNetworkingSockets.so)
