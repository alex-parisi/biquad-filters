cmake_minimum_required(VERSION 3.29)
project(biquad-filters-cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if (MSVC)
    add_compile_options(/Qspectre- /wd5045 /wd4710)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")
    if (NOT MSVC)
        set(TARGET_COMPILE_OPTIONS -fsanitize=address -fno-omit-frame-pointer -Wall)
        set(TARGET_LINK_OPTIONS -fsanitize=address)
    else()
        set(TARGET_COMPILE_OPTIONS -Wall)
        set(TARGET_LINK_OPTIONS "")
    endif()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (NOT MSVC)
        set(TARGET_COMPILE_OPTIONS -O3 -DNDEBUG -Wall)
    else()
        set(TARGET_COMPILE_OPTIONS /O2 /Qspectre- -DNDEBUG -Wall)
    endif()
    set(TARGET_LINK_OPTIONS "")
else()
    message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif ()

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

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${test_exe} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
        )
    endif ()
    target_compile_options(${test_exe} PRIVATE
            ${TARGET_COMPILE_OPTIONS}
    )
    target_link_options(${test_exe} PRIVATE
            ${TARGET_LINK_OPTIONS}
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
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(BiquadProfiler PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
endif ()
target_compile_options(BiquadProfiler PRIVATE
        ${TARGET_COMPILE_OPTIONS}
)
target_link_options(BiquadProfiler PRIVATE
        ${TARGET_LINK_OPTIONS}
)