add_library(Argon2 STATIC src/Argon2/argon2-core.cpp src/Argon2/argon2-opt-core.cpp src/Argon2/argon2-ref-core.cpp src/Argon2/argon2.cpp src/Argon2/kat.cpp
                          src/Blake2/blake2b.c)

target_include_directories(Argon2 PRIVATE include src/Blake2)
