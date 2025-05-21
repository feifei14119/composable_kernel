#add_custom_target(example_gemm_mx)
#
#add_example_executable(example_gemm_mx_fp8 gemm_mx_fp8.cpp)
#add_example_dependencies(example_gemm_mx example_gemm_mx_fp8)
#
#add_example_executable(example_gemm_mx_bf8 gemm_mx_bf8.cpp)
#add_example_dependencies(example_gemm_mx example_gemm_mx_bf8)
#
#add_example_executable(example_gemm_mx_fp8_bf8 gemm_mx_fp8_bf8.cpp)
#add_example_dependencies(example_gemm_mx example_gemm_mx_fp8_bf8)
#
#add_example_executable(example_gemm_mx_fp8_bpreshuffle gemm_mx_fp8_bpreshuffle.cpp)
#add_example_dependencies(example_gemm_mx example_gemm_mx_fp8_bpreshuffle)
#
#add_example_executable(example_gemm_mx_fp4 gemm_mx_fp4.cpp)
#add_example_dependencies(example_gemm_mx example_gemm_mx_fp4)
#
#add_example_executable(example_gemm_mx_fp4_bpreshuffle gemm_mx_fp4_bpreshuffle.cpp)
#add_example_dependencies(example_gemm_mx example_gemm_mx_fp4_bpreshuffle)
#
#add_example_executable(example_moe_gemm1_xdl_mx_fp4 moe_gemm1_xdl_mx_fp4.cpp)
#add_example_dependencies(example_gemm_mx example_moe_gemm1_xdl_mx_fp4)
#
#add_example_executable(example_moe_gemm2_xdl_mx_fp4 moe_gemm2_xdl_mx_fp4.cpp)
#add_example_dependencies(example_gemm_mx example_moe_gemm2_xdl_mx_fp4)
#
#add_example_executable(example_moe_gemm2_xdl_mx_fp4_bns moe_gemm2_xdl_mx_fp4_bns.cpp)
#add_example_dependencies(example_gemm_mx example_moe_gemm2_xdl_mx_fp4_bns)
#
#set(FP4_MXGEMM_OPTIONS)
#list(APPEND FP4_MXGEMM_OPTIONS "SHELL: -mllvm -greedy-reverse-local-assignment=1")
#list(APPEND FP4_MXGEMM_OPTIONS -v --save-temps -Wno-gnu-line-marker -ftemplate-backtrace-limit=0)
#set(FP8_MXGEMM_OPTIONS)
#list(APPEND FP8_MXGEMM_OPTIONS "SHELL: -mllvm -greedy-reverse-local-assignment=1 -mllvm --slp-threshold=-32")
#list(APPEND FP8_MXGEMM_OPTIONS -v --save-temps -Wno-gnu-line-marker -ftemplate-backtrace-limit=0)
#target_compile_options(example_gemm_mx_fp4 PRIVATE ${FP4_MXGEMM_OPTIONS})
#target_compile_options(example_gemm_mx_fp4_bpreshuffle PRIVATE ${FP4_MXGEMM_OPTIONS})
#target_compile_options(example_moe_gemm1_xdl_mx_fp4 PRIVATE ${FP4_MXGEMM_OPTIONS})
#target_compile_options(example_moe_gemm2_xdl_mx_fp4 PRIVATE ${FP4_MXGEMM_OPTIONS})
#target_compile_options(example_moe_gemm2_xdl_mx_fp4_bns PRIVATE ${FP4_MXGEMM_OPTIONS})
#
#target_compile_options(example_gemm_mx_fp8 PRIVATE ${FP8_MXGEMM_OPTIONS})



add_executable(mx EXCLUDE_FROM_ALL moe_gemm1_xdl_mx_fp4_bns.cpp)
set(EXAMPLE_FF_COMPILE_OPTIONS)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -Wno-undefined-func-template -Wno-float-equal)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -Wno-unused-variable -Wno-unused-parameter)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -Wno-unused-local-typedef)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -Wno-unreachable-code)
#list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -O0)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -v)
#list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -ggdb -g)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -save-temps)
# list(APPEND EXAMPLE_FF_COMPILE_OPTIONS  -mllvm -greedy-reverse-local-assignment=1)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS "SHELL: -mllvm -greedy-reverse-local-assignment=1 -mllvm --slp-threshold=-32")
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -Wno-gnu-line-marker)
list(APPEND EXAMPLE_FF_COMPILE_OPTIONS -DFF_DBG=1 -DFF_DBG_CNT=64)

target_link_libraries(mx PRIVATE utility)
#add_test(NAME ${EXAMPLE_NAME} COMMAND $<TARGET_FILE:${EXAMPLE_NAME}> ${ARGN})
#set_property(TARGET ${EXAMPLE_NAME} PROPERTY HIP_ARCHITECTURES ${EX_TARGETS} )
add_dependencies(examples mx)
add_dependencies(check mx)

target_compile_options(mx PRIVATE ${EXAMPLE_FF_COMPILE_OPTIONS})
