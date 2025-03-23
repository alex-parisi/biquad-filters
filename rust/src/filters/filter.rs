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
pub trait Filter<T: Float> {
    /// Processes a single sample in-place and returns a boolean indicating success.
    fn process(&mut self, sample: &mut T) -> bool;
    /// Processes a block of samples in-place and returns a boolean indicating success.
    fn process_block(&mut self, samples: &mut [T]) -> bool;
    /// Returns the current configuration of the filter.
    fn get_configuration(&self) -> FilterConfiguration<T>;
    /// Sets the configuration of the filter.
    fn set_configuration(&mut self, configuration: FilterConfiguration<T>) -> bool;
}