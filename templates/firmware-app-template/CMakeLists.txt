cmake_minimum_required(VERSION 3.16)
project(firmware_app C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
add_executable(firmware_app
    src/main.c
    src/app.c
)
target_include_directories(firmware_app PRIVATE include)
