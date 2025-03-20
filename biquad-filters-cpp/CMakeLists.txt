cmake_minimum_required(VERSION 3.29)
project(filters VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#############
### TESTS ###
#############

set(TEST_SOURCES
        DigitalBiquadFilter
        LowPassFilter
        HighPassFilter
        BandPassFilter
        NotchFilter
        AllPassFilter
        PeakingEQFilter
        LowShelfFilter
        HighShelfFilter
)

# Define a function to reduce redundancy
function(add_filter_test test_name)
    set(test_exe ${test_name}Test)
    set(test_source test/${test_name}Test.cpp)

    add_executable(${test_exe} ${test_source})

    target_include_directories(${test_exe} PRIVATE
            ${GTEST_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/inc
    )

    target_link_libraries(${test_exe} PRIVATE
            GTest::gtest_main
    )

    set_target_properties(${test_exe} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )

    add_test(NAME test_${test_name} COMMAND ${test_exe})
endfunction()

# Add all tests using the function
foreach (test ${TEST_SOURCES})
    add_filter_test(${test})
endforeach ()

################
### Profiler ###
################

set(PROFILER_SOURCES
        src/Profiler.cpp
)
add_executable(BiquadProfiler ${PROFILER_SOURCES})
target_include_directories(BiquadProfiler PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
set_target_properties(BiquadProfiler PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)