add_library(ecl_ring_buffer STATIC
    src/ecl_ring_buffer.c
)

target_include_directories(ecl_ring_buffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
)

target_compile_features(ecl_ring_buffer PUBLIC c_std_99)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ecl_ring_buffer PRIVATE -Wall -Wextra -Wpedantic)
endif()
