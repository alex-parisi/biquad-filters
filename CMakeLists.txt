cmake_minimum_required(VERSION 3.29)
project(filters VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# DigitalBiquadFilter Test
set(DIGITAL_BIQUAD_FILTER_TEST_SOURCES
        test/DigitalBiquadFilterTest.cpp
)
add_executable(DigitalBiquadFilterTest ${DIGITAL_BIQUAD_FILTER_TEST_SOURCES})
target_include_directories(DigitalBiquadFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(DigitalBiquadFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(DigitalBiquadFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(DigitalBiquadFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(DigitalBiquadFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_DigitalBiquadFilter COMMAND DigitalBiquadFilterTest)

# LowPassFilter Test
set(LOW_PASS_FILTER_TEST_SOURCES
        test/LowPassFilterTest.cpp
)
add_executable(LowPassFilterTest ${LOW_PASS_FILTER_TEST_SOURCES})
target_include_directories(LowPassFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(LowPassFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(LowPassFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(LowPassFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(LowPassFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_LowPassFilter COMMAND LowPassFilterTest)

# HighPassFilter Test
set(HIGH_PASS_FILTER_TEST_SOURCES
        test/HighPassFilterTest.cpp
)
add_executable(HighPassFilterTest ${HIGH_PASS_FILTER_TEST_SOURCES})
target_include_directories(HighPassFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(HighPassFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(HighPassFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(HighPassFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(HighPassFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_HighPassFilter COMMAND HighPassFilterTest)

# HighPassFilter Test
set(BAND_PASS_FILTER_TEST_SOURCES
        test/BandPassFilterTest.cpp
)
add_executable(BandPassFilterTest ${BAND_PASS_FILTER_TEST_SOURCES})
target_include_directories(BandPassFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(BandPassFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(BandPassFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(BandPassFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(BandPassFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_BandPassFilterTest COMMAND BandPassFilterTest)

# NotchFilter Test
set(NOTCH_FILTER_TEST_SOURCES
        test/NotchFilterTest.cpp
)
add_executable(NotchFilterTest ${NOTCH_FILTER_TEST_SOURCES})
target_include_directories(NotchFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(NotchFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(NotchFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(NotchFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(NotchFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_NotchFilterTest COMMAND NotchFilterTest)

# AllPassFilter Test
set(ALL_PASS_FILTER_TEST_SOURCES
        test/AllPassFilterTest.cpp
)
add_executable(AllPassFilterTest ${ALL_PASS_FILTER_TEST_SOURCES})
target_include_directories(AllPassFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(AllPassFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(AllPassFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(AllPassFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(AllPassFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_AllPassFilterTest COMMAND AllPassFilterTest)

# PeakingEQFilter Test
set(PEAKING_EQ_FILTER_TEST_SOURCES
        test/PeakingEQFilterTest.cpp
)
add_executable(PeakingEQFilterTest ${PEAKING_EQ_FILTER_TEST_SOURCES})
target_include_directories(PeakingEQFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(PeakingEQFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(PeakingEQFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(PeakingEQFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(PeakingEQFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_PeakingEQFilterTest COMMAND PeakingEQFilterTest)

# LowShelfFilter Test
set(LOW_SHELF_FILTER_TEST_SOURCES
        test/LowShelfFilterTest.cpp
)
add_executable(LowShelfFilterTest ${LOW_SHELF_FILTER_TEST_SOURCES})
target_include_directories(LowShelfFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(LowShelfFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(LowShelfFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(LowShelfFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(LowShelfFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_LowShelfFilterTest COMMAND LowShelfFilterTest)

# HighShelfFilter Test
set(HIGH_SHELF_FILTER_TEST_SOURCES
        test/HighShelfFilterTest.cpp
)
add_executable(HighShelfFilterTest ${HIGH_SHELF_FILTER_TEST_SOURCES})
target_include_directories(HighShelfFilterTest PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(HighShelfFilterTest PRIVATE
        GTest::gtest_main
)
target_compile_options(HighShelfFilterTest PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -Wall
)
target_link_options(HighShelfFilterTest PRIVATE
        -fsanitize=address
)
set_target_properties(HighShelfFilterTest PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
add_test(NAME test_HighShelfFilterTest COMMAND HighShelfFilterTest)