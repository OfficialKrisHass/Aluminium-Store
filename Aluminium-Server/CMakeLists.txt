message("------ Configuring Aluminium-Server ------")
message("")

# defines

set(AL_DEFINES ${AL_COMMON_DEFINES})
set(AL_DEFINES ${AL_DEFINES} AL_SERVER AL_LOGGING)

# defines

set(AL_SOURCES ${AL_COMMON_SOURCES})
set(AL_SOURCES ${AL_SOURCES} src/main.cpp src/Network.cpp src/Database.cpp src/Input.cpp)

# includes

set(AL_INCLUDES ${AL_COMMON_INCLUDES})
set(AL_INCLUDES ${AL_INCLUDES} lib/mysql-conn/include/jdbc)

# final

add_executable(Aluminium-Server ${AL_SOURCES})
set_target_properties(Aluminium-Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${OUTPUT_DIR}/Aluminium-Server")

target_compile_definitions(Aluminium-Server PRIVATE ${AL_DEFINES})
target_include_directories(Aluminium-Server PRIVATE ${AL_INCLUDES})

target_link_libraries(Aluminium-Server PRIVATE ${AL_COMMON_LIBRARIES} ${PROJECT_SOURCE_DIR}/Aluminium-Server/lib/mysql-conn/bin/Linux/libmysqlcppconn.so)
