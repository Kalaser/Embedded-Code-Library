add_library(ecl_component STATIC src/ecl_component.c)
target_include_directories(ecl_component PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(ecl_component PUBLIC c_std_99)
