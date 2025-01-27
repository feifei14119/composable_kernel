add_executable(tile_example_gemm_basic EXCLUDE_FROM_ALL gemm_basic.cpp)
add_executable(tile_example_gemm_universal EXCLUDE_FROM_ALL universal_gemm.cpp)

set(EXAMPLE_GEMM_COMPILE_OPTIONS)
list(APPEND EXAMPLE_GEMM_COMPILE_OPTIONS -Wno-undefined-func-template -Wno-float-equal)
list(APPEND EXAMPLE_GEMM_COMPILE_OPTIONS -Wno-unused-variable -Wno-unused-parameter)
list(APPEND EXAMPLE_GEMM_COMPILE_OPTIONS -ggdb -g -O0 -v -save-temps)
target_compile_options(tile_example_gemm_basic PRIVATE ${EXAMPLE_GEMM_COMPILE_OPTIONS})
