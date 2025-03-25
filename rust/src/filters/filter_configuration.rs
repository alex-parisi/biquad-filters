/// filter_configuration.rs

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
use num_traits::{Float, Zero};
use std::ops::MulAssign;


/// Configuration for a filter.
#[derive(Debug, Clone, Copy)]
pub struct FilterConfiguration<T: Float + Default> {
    cutoff: T,
    sample_rate: u32,
    q_factor: T,
    gain: T,
    constant_skirt_gain: bool,
    bypass: bool,
}

/// Implementation of FilterConfiguration.
/// TODO - Add bandwidth setting
impl<T> FilterConfiguration<T>
where
    T: Float + Default + MulAssign + Copy,
{
    pub fn new(
        cutoff: T,
        sample_rate: u32,
        q_factor: T,
        gain: T,
        constant_skirt_gain: bool,
        bypass: bool,
    ) -> Self {
        Self {
            cutoff,
            sample_rate,
            q_factor,
            gain,
            constant_skirt_gain,
            bypass,
        }
    }

    /// Returns the cutoff frequency of the filter.
    pub fn get_cutoff(&self) -> T {
        self.cutoff
    }

    /// Returns the sample rate of the filter.
    pub fn get_sample_rate(&self) -> u32 {
        self.sample_rate
    }

    /// Returns the Q factor of the filter.
    pub fn get_q_factor(&self) -> T {
        self.q_factor
    }

    /// Returns the gain of the filter.
    pub fn get_gain(&self) -> T {
        self.gain
    }

    /// Returns whether the filter should maintain a constant skirt gain.
    pub fn get_constant_skirt_gain(&self) -> bool {
        self.constant_skirt_gain
    }

    /// Sets the cutoff frequency of the filter.
    pub fn set_cutoff(&mut self, value: T) {
        self.cutoff = value;
    }

    /// Sets the sample rate of the filter.
    pub fn set_sample_rate(&mut self, value: u32) {
        self.sample_rate = value;
    }

    /// Sets the cutoff frequency of the filter.
    pub fn set_q_factor(&mut self, value: T) {
        self.q_factor = value;
    }

    /// Sets the gain of the filter.
    pub fn set_gain(&mut self, value: T) {
        self.gain = value;
    }

    /// Sets whether the filter should maintain a constant skirt gain.
    pub fn set_constant_skirt_gain(&mut self, value: bool) {
        self.constant_skirt_gain = value;
    }

    /// Sets whether the filter should bypass the filter.
    pub fn set_bypass(&mut self, value: bool) {
        self.bypass = value;
    }

    /// Returns whether the filter should bypass the filter.
    pub fn get_bypass(&self) -> bool {
        self.bypass
    }
}

/// Implementing Default for FilterConfiguration.
impl<T: Float + Default> Default for FilterConfiguration<T> {
    fn default() -> Self {
        Self {
            cutoff: T::zero(),
            sample_rate: u32::zero(),
            q_factor: T::zero(),
            gain: T::zero(),
            constant_skirt_gain: false,
            bypass: true,
        }
    }
}
