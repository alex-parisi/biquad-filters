/// biquad_tests.rs

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
use biquad_filters::{Coefficients, DigitalBiquadFilter};
use num_traits::Float;


#[test]
fn test_create_valid_double_filter() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let filter = DigitalBiquadFilter::new(coefficients);
    assert!(filter.is_some(), "Filter should be created successfully");
}

#[test]
fn test_create_valid_float_filter() {
    let coefficients = Coefficients {
        b0: 1.0_f32,
        b1: 0.0_f32,
        b2: 0.0_f32,
        a0: 1.0_f32,
        a1: 0.0_f32,
        a2: 0.0_f32,
    };

    let filter = DigitalBiquadFilter::new(coefficients);
    assert!(filter.is_some(), "Filter should be created successfully");
}

#[test]
fn test_create_invalid_double_filter() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 0.0, // Invalid a0
        a1: 0.0,
        a2: 0.0,
    };

    let filter = DigitalBiquadFilter::new(coefficients);
    assert!(
        filter.is_none(),
        "Filter creation should fail with a0 == 0.0"
    );
}

#[test]
fn test_process_single_double_sample() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut sample = 1.0;
    filter.process(&mut sample);
    assert!(
        (sample - 1.0).abs() < f64::EPSILON,
        "Sample should remain unchanged"
    );
}

#[test]
fn test_process_single_float_sample() {
    let coefficients = Coefficients {
        b0: 1.0_f32,
        b1: 0.0_f32,
        b2: 0.0_f32,
        a0: 1.0_f32,
        a1: 0.0_f32,
        a2: 0.0_f32,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut sample = 1.0_f32;
    filter.process(&mut sample);
    assert!(
        (sample - 1.0_f32).abs() < f32::EPSILON,
        "Sample should remain unchanged"
    );
}

#[test]
fn test_process_block_of_double_samples() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut samples = [1.0, 0.5, 0.25];
    filter.process_block(&mut samples);

    assert!((samples[0] - 1.0).abs() < f64::EPSILON);
    assert!((samples[1] - 0.5).abs() < f64::EPSILON);
    assert!((samples[2] - 0.25).abs() < f64::EPSILON);
}

#[test]
fn test_reset_filter() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut sample = 1.0;
    filter.process(&mut sample);
    filter.reset();
    let mut new_sample = 1.0;
    filter.process(&mut new_sample);

    assert!(
        (new_sample - 1.0).abs() < f64::EPSILON,
        "Reset filter should process sample correctly"
    );
}

#[test]
fn test_process_with_zero_coefficients() {
    let coefficients = Coefficients {
        b0: 0.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut sample = 1.0;
    filter.process(&mut sample);

    assert!(
        sample.abs() < f64::EPSILON,
        "Sample should be 0 when coefficients are zero"
    );
}

#[test]
fn test_process_ramp_signal() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut samples = [0.0, 1.0, 2.0, 3.0, 4.0];
    filter.process_block(&mut samples);

    assert!((samples[0] - 0.0).abs() < f64::EPSILON);
    assert!((samples[1] - 1.0).abs() < f64::EPSILON);
    assert!((samples[2] - 2.0).abs() < f64::EPSILON);
    assert!((samples[3] - 3.0).abs() < f64::EPSILON);
    assert!((samples[4] - 4.0).abs() < f64::EPSILON);
}

#[test]
fn test_process_large_values() {
    let coefficients = Coefficients {
        b0: 1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut large_sample = 1e6;
    filter.process(&mut large_sample);

    assert!(
        (large_sample - 1e6).abs() < f64::EPSILON,
        "Large sample should remain unchanged"
    );
}

#[test]
fn test_process_with_negative_coefficients() {
    let coefficients = Coefficients {
        b0: -1.0,
        b1: 0.0,
        b2: 0.0,
        a0: 1.0,
        a1: 0.0,
        a2: 0.0,
    };

    let mut filter = DigitalBiquadFilter::new(coefficients).expect("Filter creation failed");
    let mut sample = 1.0;
    filter.process(&mut sample);

    assert!(
        (sample + 1.0).abs() < f64::EPSILON,
        "Sample should be inverted due to negative coefficient"
    );
}
