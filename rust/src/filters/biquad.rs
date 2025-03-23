/// biquad.rs

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
use std::ops::MulAssign;


/// Coefficients struct for the digital biquad filter.
#[derive(Debug, Clone, Copy)]
pub struct Coefficients<T: Float> {
    pub b0: T,
    pub b1: T,
    pub b2: T,
    pub a0: T,
    pub a1: T,
    pub a2: T,
}

/// State struct for storing the filter's internal state.
#[derive(Debug, Clone, Copy)]
pub struct State<T: Float + Default> {
    pub x1: T,
    pub x2: T,
    pub y1: T,
    pub y2: T,
}

/// Digital Biquad Filter implementation.
#[derive(Debug, Clone)]
pub struct DigitalBiquadFilter<T: Float + Default> {
    coefficients: Coefficients<T>,
    state: State<T>,
    iter: u64,
}

impl<T> DigitalBiquadFilter<T>
where
    T: Float + Default + MulAssign + Copy,
{
    /// Creates a new filter instance with the given coefficients.
    pub fn new(coefficients: Coefficients<T>) -> Option<Self> {
        if coefficients.a0.is_zero() {
            return None;
        }
        let mut filter = Self {
            coefficients,
            state: State::default(),
            iter: 0,
        };
        filter.normalize_coefficients();
        Some(filter)
    }

    /// Processes a single sample.
    pub fn process(&mut self, sample: &mut T) -> bool {
        let output = self.coefficients.b0 * *sample
            + self.coefficients.b1 * self.state.x1
            + self.coefficients.b2 * self.state.x2
            - self.coefficients.a1 * self.state.y1
            - self.coefficients.a2 * self.state.y2;

        self.state.x2 = self.state.x1;
        self.state.x1 = *sample;
        self.state.y2 = self.state.y1;
        self.state.y1 = output;
        *sample = output;

        self.iter += 1;
        true
    }

    /// Processes a block of samples.
    pub fn process_block(&mut self, samples: &mut [T]) -> bool {
        if samples.is_empty() {
            return false;
        }
        for sample in samples.iter_mut() {
            self.process(sample);
        }
        true
    }

    /// Sets new coefficients for the filter.
    pub fn set_coefficients(&mut self, coefficients: Coefficients<T>) -> bool {
        if coefficients.a0.is_zero() {
            return false;
        }
        self.coefficients = coefficients;
        self.normalize_coefficients();
        self.reset();
        true
    }

    /// Resets the filter state.
    pub fn reset(&mut self) {
        self.state = State::default();
        self.iter = u64::default();
    }

    /// Normalizes the coefficients by dividing all by a0.
    fn normalize_coefficients(&mut self) {
        let a0_inv = T::one() / self.coefficients.a0;
        self.coefficients.b0 *= a0_inv;
        self.coefficients.b1 *= a0_inv;
        self.coefficients.b2 *= a0_inv;
        self.coefficients.a1 *= a0_inv;
        self.coefficients.a2 *= a0_inv;
        self.coefficients.a0 = T::one();
    }
}

impl<T: Float + Default> Default for State<T> {
    fn default() -> Self {
        Self {
            x1: T::zero(),
            x2: T::zero(),
            y1: T::zero(),
            y2: T::zero(),
        }
    }
}
