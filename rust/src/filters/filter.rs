use crate::filters::biquad::{Coefficients, DigitalBiquadFilter};
use crate::filters::filter_configuration::FilterConfiguration;
/// filter.rs

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
use num_traits::Float;

pub trait BiquadFilterWrapper<T: Float + Default + Copy + std::ops::MulAssign> {
    fn get_filter(&mut self) -> &mut DigitalBiquadFilter<T>;
    fn get_config(&self) -> &FilterConfiguration<T>;
    fn get_config_mut(&mut self) -> &mut FilterConfiguration<T>;
    fn calculate_coefficients(config: &FilterConfiguration<T>) -> Option<Coefficients<T>>;
}

/// A Generic Filter trait for processing audio samples.
pub trait Filter<T: Float + Default> {
    /// Processes a single sample in-place and returns a boolean indicating success.
    fn process(&mut self, sample: &mut T) -> bool;
    /// Processes a block of samples in-place and returns a boolean indicating success.
    fn process_block(&mut self, samples: &mut [T]) -> bool;
    /// Returns the current configuration of the filter.
    fn get_configuration(&self) -> FilterConfiguration<T>;
    /// Sets the configuration of the filter.
    fn set_configuration(&mut self, configuration: FilterConfiguration<T>) -> bool;
    /// Returns the cutoff frequency of the filter.
    fn get_cutoff(&self) -> T;
    /// Sets the cutoff frequency of the filter.
    fn set_cutoff(&mut self, cutoff: T) -> bool;
    /// Returns the sample rate of the filter.
    fn get_sample_rate(&self) -> u32;
    /// Sets the sample rate of the filter.
    fn set_sample_rate(&mut self, sample_rate: u32) -> bool;
    /// Returns the Q factor of the filter.
    fn get_q_factor(&self) -> T;
    /// Sets the Q factor of the filter.
    fn set_q_factor(&mut self, q_factor: T) -> bool;
    /// Returns the gain of the filter.
    fn get_gain(&self) -> T;
    /// Sets the gain of the filter.
    fn set_gain(&mut self, gain: T) -> bool;
    /// Returns whether the filter has a constant skirt gain.
    fn get_constant_skirt_gain(&self) -> bool;
    /// Sets whether the filter should have a constant skirt gain.
    fn set_constant_skirt_gain(&mut self, constant_skirt_gain: bool) -> bool;
    /// Returns whether the filter should be bypassed.
    fn get_bypass(&self) -> bool;
    /// Sets whether the filter should be bypassed.
    fn set_bypass(&mut self, bypass: bool) -> bool;
}

impl<T, F> Filter<T> for F
where
    T: Float + Default + Copy + std::ops::MulAssign,
    F: BiquadFilterWrapper<T>,
{
    /// Processes a single sample in-place and returns a boolean indicating success.
    fn process(&mut self, sample: &mut T) -> bool {
        if self.get_config().get_bypass() {
            return true;
        }
        self.get_filter().process(sample)
    }

    /// Processes a block of samples in-place and returns a boolean indicating success.
    fn process_block(&mut self, samples: &mut [T]) -> bool {
        if self.get_config().get_bypass() {
            return true;
        }
        self.get_filter().process_block(samples)
    }

    /// Returns the current configuration of the filter.
    fn get_configuration(&self) -> FilterConfiguration<T> {
        self.get_config().clone()
    }

    /// Sets the configuration of the filter.
    fn set_configuration(&mut self, config: FilterConfiguration<T>) -> bool {
        *self.get_config_mut() = config;
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }

    /// Returns the cutoff frequency of the filter.
    fn get_cutoff(&self) -> T {
        self.get_config().get_cutoff()
    }

    /// Sets the cutoff frequency of the filter.
    fn set_cutoff(&mut self, cutoff: T) -> bool {
        self.get_config_mut().set_cutoff(cutoff);
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }

    /// Returns the sample rate of the filter.
    fn get_sample_rate(&self) -> u32 {
        self.get_config().get_sample_rate()
    }

    /// Sets the sample rate of the filter.
    fn set_sample_rate(&mut self, rate: u32) -> bool {
        self.get_config_mut().set_sample_rate(rate);
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }

    /// Returns the Q factor of the filter.
    fn get_q_factor(&self) -> T {
        self.get_config().get_q_factor()
    }

    /// Sets the Q factor of the filter.
    fn set_q_factor(&mut self, q: T) -> bool {
        self.get_config_mut().set_q_factor(q);
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }

    /// Returns the gain of the filter. This is only applicable for peaking and shelving filters.
    fn get_gain(&self) -> T {
        self.get_config().get_gain()
    }

    /// Sets the gain of the filter. This is only applicable for peaking and shelving filters.
    /// If this parameter is not applicable for the current filter type, this will do nothing.
    fn set_gain(&mut self, gain: T) -> bool {
        self.get_config_mut().set_gain(gain);
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }

    /// Returns whether the filter has a constant skirt gain. This is only applicable for band-pass
    /// filters.
    fn get_constant_skirt_gain(&self) -> bool {
        self.get_config().get_constant_skirt_gain()
    }

    /// Sets whether the filter should have a constant skirt gain. This is only applicable for
    /// band-pass filters. If this parameter is not applicable for the current filter type, this
    /// will do nothing.
    fn set_constant_skirt_gain(&mut self, constant_skirt_gain: bool) -> bool {
        self.get_config_mut()
            .set_constant_skirt_gain(constant_skirt_gain);
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }

    /// Returns whether the filter should be bypassed.
    fn get_bypass(&self) -> bool {
        self.get_config().get_bypass()
    }

    /// Sets whether the filter should be bypassed.
    fn set_bypass(&mut self, bypass: bool) -> bool {
        self.get_config_mut().set_bypass(bypass);
        if let Some(coefficients) = Self::calculate_coefficients(self.get_config()) {
            self.get_filter().set_coefficients(coefficients)
        } else {
            false
        }
    }
}
