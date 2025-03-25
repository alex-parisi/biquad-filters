# Digital Biquad Filters

This repository contains a collection of digital biquad filters implemented in
Rust.

The filters are based off the C++ implementation, which can be found [here](https://github.com/alex-parisi/biquad-filters/tree/main/cpp).

---

### Brief:

For information on biquad filters, you can check out my
website [here](https://atparisi.com/html/digitalBiquadFilter.html).

---

### Usage:

To use the filters, simply add `biquad-filters-rust` to your Rust project,
create an instance of the filter, and process your data:

```
/// Import the biquad_filters crate
use biquad_filters::{Filter, LowPassFilter};

/// Define some dummy data to operate on
let n: usize = 100;
let mut samples: Vec<f64> = vec![0.0_f64; n];

/// Create the filter
/// This is a low-pass filter with a cutoff frequency at 1000 Hz, 
/// a sample rate of 44100 Hz, and a Q factor of 0.707
let filter = LowPassFilter::<f64>::new(
    1000.0_f64,
    44100_u32,
    std::f64::consts::FRAC_1_SQRT_2
).expect("Failed to create filter");

/// Process the data in-place
filter.process_block(&mut samples);
```

When using the filter's `::new` function, it returns an `Option` so
you can check if the filter was created successfully.

---

### Supported Filters:

- Generic Digital Biquad
- Low Pass
- High Pass
- Band Pass
- Notch
- All Pass
- Peaking EQ
- Low Shelf
- High Shelf

---

### Notes:

- It's always recommended to template the filters as `f64` for the best
  precision and to reduce the chance of encountering quantization noise. While
  processing blocks of `f32` samples will be faster, the precision of the
  filter will be reduced.