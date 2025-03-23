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
use crate::filters::filter_configuration::FilterConfiguration;

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
}