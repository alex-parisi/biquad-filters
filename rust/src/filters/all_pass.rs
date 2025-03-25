/// all_pass.rs

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
use crate::filters::biquad::{Coefficients, DigitalBiquadFilter};
use crate::filters::filter::BiquadFilterWrapper;
use crate::filters::filter_configuration::FilterConfiguration;
use num_traits::Float;
use std::f64::consts::PI;


/// All-pass filter implementation using a digital biquad filter.
#[derive(Debug, Clone)]
pub struct AllPassFilter<T: Float + Default + Copy> {
    /// The digital biquad filter used for processing.
    filter: DigitalBiquadFilter<T>,
    /// The configuration for the filter, including cutoff frequency, sample rate, and Q factor.
    config: FilterConfiguration<T>,
}

impl<T: Float + Default + Copy + std::ops::MulAssign> AllPassFilter<T> {
    /// Creates a new all-pass filter with the given cutoff frequency, sample rate, and Q factor.
    pub fn new(cutoff: T, sample_rate: u32, q_factor: T) -> Option<Self> {
        let config =
            FilterConfiguration::new(
                cutoff,
                sample_rate,
                q_factor,
                T::zero(),
                false,
                false
            );
        let coefficients = Self::calculate_coefficients(&config)?;
        let filter = DigitalBiquadFilter::new(coefficients)?;
        Some(Self { filter, config })
    }
}

/// Provide internal access and coefficient logic via BiquadFilterWrapper.
impl<T: Float + Default + Copy + std::ops::MulAssign> BiquadFilterWrapper<T> for AllPassFilter<T> {
    fn get_filter(&mut self) -> &mut DigitalBiquadFilter<T> {
        &mut self.filter
    }

    fn get_config(&self) -> &FilterConfiguration<T> {
        &self.config
    }

    fn get_config_mut(&mut self) -> &mut FilterConfiguration<T> {
        &mut self.config
    }

    fn calculate_coefficients(config: &FilterConfiguration<T>) -> Option<Coefficients<T>> {
        let cutoff = config.get_cutoff();
        let sample_rate = config.get_sample_rate();
        let q = config.get_q_factor();

        if cutoff <= T::zero() || sample_rate == 0 || q <= T::zero() {
            return None;
        }

        let two = T::from(2.0)?;
        let pi = T::from(PI)?;
        let one = T::one();

        let w0 = two * pi * cutoff / T::from(sample_rate)?;
        let cos_w0 = w0.cos();
        let sin_w0 = w0.sin();
        let alpha = sin_w0 / (two * q);

        let b0 = one - alpha;
        let b1 = -two * cos_w0;
        let b2 = one + alpha;
        let a0 = one + alpha;
        let a1 = -two * cos_w0;
        let a2 = one - alpha;

        Some(Coefficients {
            b0,
            b1,
            b2,
            a0,
            a1,
            a2,
        })
    }
}
