/// Profiler.cpp

/**
Copyright © 2025 Alex Parisi

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <chrono>
#include <iostream>
#include <vector>
#include <array>

#include "DigitalBiquadFilter.h"

/**
 * @brief Benchmark the creation of a valid filter
 */
template<typename T, typename... Args>
auto benchmark_creation(const int N, Args &&...args)
        -> std::chrono::duration<double> {
    std::chrono::duration<double> result{};
    for (int i = 0; i < N; i++) {
        const auto start = std::chrono::high_resolution_clock::now();
        [[maybe_unused]] auto instance =
                T::create(std::forward<Args>(args)...);
        const auto end = std::chrono::high_resolution_clock::now();
        result += end - start;
    }
    return result / N;
}

/**
 * @brief Benchmark the processing of one sample
 */
template<typename T, typename... Args>
auto benchmark_process(const int N, Args &&...args)
        -> std::chrono::duration<double> {
    std::chrono::duration<double> result{};
    T instance = T::create(std::forward<Args>(args)...).value();
    auto benchmark = [&](auto &sample) {
        for (int i = 0; i < N; i++) {
            const auto start = std::chrono::high_resolution_clock::now();
            instance.process(sample);
            const auto end = std::chrono::high_resolution_clock::now();
            result += end - start;
        }
    };
    using ValueType =
            typename std::remove_pointer_t<decltype(instance)>::value_type;
    if constexpr (std::is_same_v<ValueType, double>) {
        double sample = 0.0;
        benchmark(sample);
    } else if constexpr (std::is_same_v<ValueType, float>) {
        float sample = 0.0f;
        benchmark(sample);
    }
    return result / N;
}

/**
 * @brief Benchmark the processing of a block of samples
 */
template<typename T, typename... Args>
auto benchmark_block_process(const int N, const int blockSize, Args &&...args)
        -> std::chrono::duration<double> {
    std::chrono::duration<double> result{};
    T instance = T::create(std::forward<Args>(args)...).value();
    auto benchmark = [&](auto *samples) {
        for (int i = 0; i < N; i++) {
            const auto start = std::chrono::high_resolution_clock::now();
            instance.process(samples, blockSize);
            const auto end = std::chrono::high_resolution_clock::now();
            result += end - start;
        }
    };
    using ValueType =
            typename std::remove_pointer_t<decltype(instance)>::value_type;
    if constexpr (std::is_same_v<ValueType, double>) {
        std::vector<double> samples(blockSize, 0.0);
        benchmark(samples.data());
    } else if constexpr (std::is_same_v<ValueType, float>) {
        std::vector<float> samples(blockSize, 0.0f);
        benchmark(samples.data());
    }
    return result / N;
}

auto main() -> int {
    constexpr int N = 100000;
    constexpr std::array<int, 10> blockSizes = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};

    constexpr Coefficients<double> doubleCoefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    constexpr Coefficients<float> floatCoefficients = {.b0 = 1.0f,
                                                       .b1 = 0.0f,
                                                       .b2 = 0.0f,
                                                       .a0 = 1.0f,
                                                       .a1 = 0.0f,
                                                       .a2 = 0.0f};

    /// Profile the DigitalBiquadFilter<double> class
    std::cout << "|---------------------------------------------------\n";
    std::cout << "| Benchmarking DigitalBiquadFilter<double> class:\n";
    std::cout << "|\tN = " << N << "\n";
    std::cout << "|---------------------------------------------------\n";
    {
        const auto avgTime = benchmark_creation<DigitalBiquadFilter<double>>(
                N, doubleCoefficients);
        const auto avgTimeNs =
                std::chrono::duration_cast<std::chrono::nanoseconds>(avgTime)
                        .count();
        std::cout << "| Creation = " << avgTimeNs << " ns\n";
    }
    {
        const auto avgTime =
                benchmark_process<DigitalBiquadFilter<double>>(
                        N, doubleCoefficients);
        const auto avgTimeNs =
                std::chrono::duration_cast<std::chrono::nanoseconds>(avgTime)
                        .count();
        std::cout << "| Process = " << avgTimeNs << " ns\n";
    }
    for (const int blockSize : blockSizes) {
        const auto avgTime =
                benchmark_block_process<DigitalBiquadFilter<double>>(
                        N, blockSize, doubleCoefficients);
        const auto avgTimeNs =
                std::chrono::duration_cast<std::chrono::nanoseconds>(avgTime)
                        .count();
        std::cout << "| Block Process (" << blockSize << ") = " << avgTimeNs
                  << " ns\n";
    }

    /// Profile the DigitalBiquadFilter<float> class
    std::cout << "|---------------------------------------------------\n";
    std::cout << "| Benchmarking DigitalBiquadFilter<float> class:\n";
    std::cout << "|\tN = " << N << "\n";
    std::cout << "|---------------------------------------------------\n";
    {
        const auto avgTime = benchmark_creation<DigitalBiquadFilter<float>>(
                N, floatCoefficients);
        const auto avgTimeNs =
                std::chrono::duration_cast<std::chrono::nanoseconds>(avgTime)
                        .count();
        std::cout << "| Creation = " << avgTimeNs << " ns\n";
    }
    {
        const auto avgTime =
                benchmark_process<DigitalBiquadFilter<float>>(
                        N, floatCoefficients);
        const auto avgTimeNs =
                std::chrono::duration_cast<std::chrono::nanoseconds>(avgTime)
                        .count();
        std::cout << "| Process = " << avgTimeNs << " ns\n";
    }
    for (const int blockSize : blockSizes) {
        const auto avgTime =
                benchmark_block_process<DigitalBiquadFilter<float>>(
                        N, blockSize, floatCoefficients);
        const auto avgTimeNs =
                std::chrono::duration_cast<std::chrono::nanoseconds>(avgTime)
                        .count();
        std::cout << "| Block Process (" << blockSize << ") = " << avgTimeNs
                  << " ns\n";
    }

    return 0;
}
