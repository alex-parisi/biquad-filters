/// high_pass.rs

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
use crate::filters::filter::Filter;
use crate::filters::filter_configuration::FilterConfiguration;
use num_traits::Float;
use std::f64::consts::PI;

/// High-pass filter implementation using a digital biquad filter.
pub struct HighPassFilter<T: Float + Default + Copy> {
    filter: DigitalBiquadFilter<T>,
    config: FilterConfiguration<T>,
}

impl<T: Float + Default + Copy + std::ops::MulAssign> HighPassFilter<T> {
    /// Creates a new high-pass filter with the given cutoff frequency, sample rate, and Q factor.
    pub fn new(config: FilterConfiguration<T>) -> Option<Self> {
        if config.get_cutoff() <= T::zero() || config.get_sample_rate() <= u32::default() || config.get_q_factor() <= T::zero() {
            return None;
        }
        let coefficients = Self::calculate_coefficients(config.get_cutoff(), config.get_sample_rate(), config.get_q_factor())?;
        let filter = DigitalBiquadFilter::new(coefficients)?;
        Some(Self {
            filter,
            config
        })
    }

    /// Calculates the coefficients for the high-pass filter based on the cutoff frequency, sample
    /// rate, and Q factor.
    fn calculate_coefficients(cutoff: T, sample_rate: u32, q: T) -> Option<Coefficients<T>> {
        let two = T::from(2.0).unwrap();
        let pi = T::from(PI).unwrap();
        let one = T::one();

        let w0 = two * pi * cutoff / T::from(sample_rate)?;
        let cos_w0 = w0.cos();
        let sin_w0 = w0.sin();
        let alpha = sin_w0 / (two * q);

        let b1 = -(one + cos_w0);
        let b0 = -b1 / two;
        let b2 = b0;
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

/// Implementing the Filter trait for HighPassFilter.
impl<T: Float + Default + Copy + std::ops::MulAssign> Filter<T> for HighPassFilter<T> {
    fn process(&mut self, sample: &mut T) -> bool {
        self.filter.process(sample)
    }
    fn process_block(&mut self, samples: &mut [T]) -> bool {
        self.filter.process_block(samples)
    }

    fn get_configuration(&self) -> FilterConfiguration<T> {
        self.config.clone()
    }

    fn set_configuration(&mut self, configuration: FilterConfiguration<T>) -> bool {
        self.config = configuration;
        let coefficients = Self::calculate_coefficients(self.config.get_cutoff(), self.config.get_sample_rate(), self.config.get_q_factor());
        if coefficients.is_none() {
            return false;
        }
        self.filter.set_coefficients(coefficients.unwrap())
    }
}
