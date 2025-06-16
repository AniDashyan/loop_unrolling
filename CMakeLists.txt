cmake_minimum_required(VERSION 3.10)
project(SumSquares)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPT_LEVELS "O0" "O2" "O3")

foreach(OPT_LEVEL ${OPT_LEVELS})
    add_executable(sum_squares_${OPT_LEVEL} main.cpp)
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # MSVC-specific flags
        if(OPT_LEVEL STREQUAL "O0")
            target_compile_options(sum_squares_${OPT_LEVEL} PRIVATE /Od /Zi)
        elseif(OPT_LEVEL STREQUAL "O2")
            target_compile_options(sum_squares_${OPT_LEVEL} PRIVATE /O2 /Zi)
        elseif(OPT_LEVEL STREQUAL "O3")
            target_compile_options(sum_squares_${OPT_LEVEL} PRIVATE /Ox /Zi)
        endif()
    else()
        # GCC/Clang-specific flags
        target_compile_options(sum_squares_${OPT_LEVEL} PRIVATE -${OPT_LEVEL} -g)
    endif()
    
    set_target_properties(sum_squares_${OPT_LEVEL} PROPERTIES OUTPUT_NAME "sum_squares_${OPT_LEVEL}")
endforeach()

# Create a custom target to run all executables
add_custom_target(run_all
    COMMENT "Running all optimization levels"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS sum_squares_O0 sum_squares_O2 sum_squares_O3
)

foreach(OPT_LEVEL ${OPT_LEVELS})
    add_custom_command(TARGET run_all
        COMMAND ${CMAKE_COMMAND} -E echo "Running with -${OPT_LEVEL} optimization"
        COMMAND sum_squares_${OPT_LEVEL}
        COMMAND ${CMAKE_COMMAND} -E echo ""
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach()