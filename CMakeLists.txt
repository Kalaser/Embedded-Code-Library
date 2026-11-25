cmake_minimum_required(VERSION 3.16)
project(embedded_code_library C)

option(ECL_BUILD_TESTS "Build host-side component tests" ON)
option(ECL_BUILD_EXAMPLES "Build host-side examples" ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(components/ring_buffer)

if(ECL_BUILD_EXAMPLES)
    add_executable(ecl_ring_buffer_demo examples/ring_buffer_demo.c)
    target_link_libraries(ecl_ring_buffer_demo PRIVATE ecl_ring_buffer)
endif()

enable_testing()
if(ECL_BUILD_TESTS)
    add_executable(test_ring_buffer components/ring_buffer/tests/test_ring_buffer.c)
    target_link_libraries(test_ring_buffer PRIVATE ecl_ring_buffer)
    add_test(NAME ring_buffer COMMAND test_ring_buffer)
endif()
