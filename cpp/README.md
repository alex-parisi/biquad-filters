# Digital Biquad Filters

This repository contains a collection of digital biquad filters implemented in
modern C++. 

The filters are implemented as a template class, which allows for
easy use with different data types. The filters are implemented in a header-only
style, so no linking is required.

---

### Brief:

For information on biquad filters, you can check out my
website [here](https://atparisi.com/html/digitalBiquadFilter.html).

---

### Usage:

To use the filters, simply download the latest release here, include the 
header files in your project, and create an instance of the filter. 

Here is an example of how to use a filter:

```cpp
#include "LowPassFilter.h"

/// Create a low pass filter with a cutoff frequency of 1000 Hz
/// and a sample rate of 44100 Hz
auto filter = LowPassFilter<double>::create(1000.0, 44100);

if (filter.has_value()) {

    /// Process a vector of samples in-place:
    std::vector<float> samplesVector = {1.0f, 0.5f, 0.25f};
    filter->process(samplesVector);

    /// Process an array of samples in-place:
    std::array<float, 3> samplesArray = {1.0f, 0.5f, 0.25f};
    filter->process(samplesArray.data(), samplesArray.size());
    
}
```

When using the filter's `::create` function, it returns an `std::optional` so
you can check if the filter was created successfully. 

If the filter was not
created successfully, the `std::optional` will be empty. This is useful to
prevent initializing a filter with invalid parameters.

If the filter is invalid, and you try to process data with it, the filter will
not modify the data and will return false.

---

### Building:

When building, it's recommended to use release flags to optimize the code. You
should add the following flags to your compiler:
```
-O3 -DNDEBUG -Wall
```
You can also use the CMake `release` profile to accomplish the same thing:
```
--preset release
```

Block operations can also be significantly faster if SIMD instructions are
enabled. To enable SIMD instructions, you can add the following flags:
```
Intel CPUs:

    SSE:     -msse4.1 
    AVX:     -mavx2
    
ARM CPUs: 

    NEON:    -mfpu=neon
```
Occasionally, the compiler will automatically enable SIMD instructions if it
detects that the CPU supports them. However, it's always best to enable them
manually.

The improvements are noticeable when processing blocks of samples, regardless
of size. For example, on my MacBook Air (M2, 2023, 16GB RAM):
```
32 float samples:
    - Without SIMD:  0.000163 ms
    - With SIMD:     0.000015 ms --> 10.87x faster
    
32 double samples:
    - Without SIMD:  0.000179 ms
    - With SIMD:     0.000040 ms --> 4.47x faster

65,536 float samples:
    - Without SIMD:  0.382619 ms
    - With SIMD:     0.054970 ms --> 6.96x faster
    
65,536 double samples:
    - Without SIMD:  0.381561 ms
    - With SIMD:     0.108450 ms --> 3.52x faster
```

Times were calculated using the `std::chrono` library and the 
`high_resolution_clock`, and averaged over 100,000 iterations.

---

### Profiling:

Optionally, you can profile your hardware by cloning this repository, building,
and running the `BiquadProfiler` executable:

```
cmake -S . -B build --preset release
cmake --build build --target BiquadProfiler -j
./build/BiquadProfiler
```

Since each filter type is a wrapper around the `DigitalBiquadFilter` class, the
profiler just measures the performance of the `DigitalBiquadFilter` class.

---

### Supported Filters:

- Generic Digital Biquad Filter
- Low Pass Filter
- High Pass Filter
- Band Pass Filter
- Notch Filter
- All Pass Filter
- Peaking Filter
- Low Shelf Filter
- High Shelf Filter

---

### Notes:

- It's always recommended to template the filters as `double` for the best
  precision to and reduce the chance of encountering quantization noise. While
  processing blocks of `float` samples will be faster, the precision of the
  filter will be reduced.