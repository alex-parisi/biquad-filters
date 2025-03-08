# Digital Biquad Filters

This repository contains a collection of digital biquad filters implemented in modern C++. The filters are implemented as a template class, which allows for easy use with different data types. The filters are implemented in a header-only style, so no linking is required.

---

### Brief:

For information on biquad filters, you can check out my website [here](https://atparisi.com/html/digitalBiquadFilter.html).

---

### Usage:

To use the filters, simply include the header files in your project and create an instance of the filter. Here is an example of how to use a filter:

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

When using the filter's `::create` function, it returns an `std::optional` so you can check if the filter was created successfully. If the filter was not created successfully, the `std::optional` will be empty. This is useful to prevent initializing a filter with invalid parameters.

If the filter is invalid, and you try to process data with it, the filter will not modify the data and will return false.

---

### Notes:

- It's always recommended to template the filters as `double` for the best precision to and reduce the chance of encountering quantization noise.
- The safest and fastest way to process a block of data is by templating the filter with a block size and passing in an array with the same block size.
    - If the block size is not templated, you can still process a block of data by passing in a pointer to the data and the size of the block. When using arrays, this is still usually safe. 