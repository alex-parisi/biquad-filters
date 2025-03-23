/// low_pass_tests.rs

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
use biquad_filters::filters::filter::Filter;
use biquad_filters::filters::low_pass::LowPassFilter;
use approx::assert_relative_eq;

#[test]
fn create_valid_double_filter() {
    let filter = LowPassFilter::<f64>::new(
        1000.0_f64,
        44100_u32,
        std::f64::consts::FRAC_1_SQRT_2
    );
    assert!(filter.is_some());
}

#[test]
fn create_valid_float_filter() {
    let filter = LowPassFilter::<f32>::new(
        1000.0_f32,
        44100_u32,
        std::f32::consts::FRAC_1_SQRT_2
    );
    assert!(filter.is_some());
}

#[test]
fn create_invalid_double_filter() {
    let filter = LowPassFilter::<f64>::new(
        1000.0_f64,
        0_u32,
        std::f64::consts::FRAC_1_SQRT_2
    );
    assert!(filter.is_none());
}

#[test]
fn create_invalid_float_filter() {
    let filter = LowPassFilter::<f32>::new(
        1000.0_f32,
        0_u32,
        std::f32::consts::FRAC_1_SQRT_2
    );
    assert!(filter.is_none());
}

#[test]
fn set_cutoff_frequency() {
    let mut filter = LowPassFilter::<f64>::new(
        1000.0_f64,
        44100_u32,
        std::f64::consts::FRAC_1_SQRT_2
    ).unwrap();
    let mut config = filter.get_configuration();
    assert_relative_eq!(config.get_cutoff(), 1000.0_f64);
    config.set_cutoff(2000.0_f64);
    filter.set_configuration(config);
    let new_config = filter.get_configuration();
    assert_relative_eq!(new_config.get_cutoff(), 2000.0_f64);
}

#[test]
fn set_sample_rate() {
    let mut filter = LowPassFilter::<f64>::new(
        1000.0_f64,
        44100_u32,
        std::f64::consts::FRAC_1_SQRT_2
    ).unwrap();
    let mut config = filter.get_configuration();
    assert_eq!(config.get_sample_rate(), 44100_u32);
    config.set_sample_rate(48000_u32);
    filter.set_configuration(config);
    let new_config = filter.get_configuration();
    assert_eq!(new_config.get_sample_rate(), 48000_u32);
}

#[test]
fn set_quality_factor() {
    let mut filter = LowPassFilter::<f64>::new(
        1000.0_f64,
        44100_u32,
        std::f64::consts::FRAC_1_SQRT_2
    ).unwrap();
    let mut config = filter.get_configuration();
    assert_relative_eq!(config.get_q_factor(), std::f64::consts::FRAC_1_SQRT_2);
    config.set_q_factor(1.0_f64);
    filter.set_configuration(config);
    let new_config = filter.get_configuration();
    assert_relative_eq!(new_config.get_q_factor(), 1.0_f64);
}
