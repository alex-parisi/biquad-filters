# biquad.py

"""
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
"""

from dataclasses import dataclass
from typing import Optional, TypeVar, Generic
import numpy as np

T = TypeVar("T", float, np.floating)


@dataclass
class Coefficients(Generic[T]):
    """
    Coefficients for a digital biquad filter.

    :param float b0: Numerator coefficient b0.
    :param float b1: Numerator coefficient b1.
    :param float b2: Numerator coefficient b2.
    :param float a0: Denominator coefficient a0.
    :param float a1: Denominator coefficient a1.
    :param float a2: Denominator coefficient a2.
    """
    b0: T
    b1: T
    b2: T
    a0: T
    a1: T
    a2: T


@dataclass
class State(Generic[T]):
    """
    State for a digital biquad filter.

    :param float x1: x[n-1] state.
    :param float x2: x[n-2] state.
    :param float y1: y[n-1] state.
    :param float y2: y[n-2] state.
    """
    x1: T = 0.0
    x2: T = 0.0
    y1: T = 0.0
    y2: T = 0.0


class DigitalBiquadFilter(Generic[T]):
    """
    Digital biquad filter implementation.

    :param Coefficients coefficients: Coefficients for the filter.
    """

    def __init__(self, coefficients: Coefficients[T]):
        """
        Initialize the digital biquad filter with the given coefficients.
        :param coefficients: Coefficients for the filter.
        """
        if coefficients.a0 == 0.0:
            raise ValueError("a0 cannot be zero.")
        self.coefficients = coefficients
        self._normalize_coefficients()
        self.state = State()
        self.iter = 0

    @staticmethod
    def create(coefficients: Coefficients[T]) -> Optional['DigitalBiquadFilter']:
        """
        Create a digital biquad filter from the given coefficients.

        :param Coefficients coefficients: Coefficients for the filter.
        """
        if coefficients.a0 == 0.0:
            return None
        return DigitalBiquadFilter(coefficients)

    def process(self, sample: T) -> T:
        """
        Process a single sample through the filter.

        :param sample: Input sample.
        :return: Filtered output.
        """
        x0 = sample
        x1 = self.state.x1
        x2 = self.state.x2
        y1 = self.state.y1
        y2 = self.state.y2

        output = (self.coefficients.b0 * x0 +
                  self.coefficients.b1 * x1 +
                  self.coefficients.b2 * x2 -
                  self.coefficients.a1 * y1 -
                  self.coefficients.a2 * y2)

        self.state.x2 = x1
        self.state.x1 = x0
        self.state.y2 = y1
        self.state.y1 = output

        self.iter += 1
        return output

    def process_block(self, samples: np.ndarray) -> np.ndarray:
        """
        Process a block of samples through the filter.

        :param samples: Input samples.
        :return: Filtered output.
        """
        if samples is None or len(samples) == 0:
            return np.ndarray(0)
        processed = np.empty_like(samples)
        for i, s in enumerate(samples):
            processed[i] = self.process(s)
        return processed

    def set_coefficients(self, coefficients: Coefficients[T]) -> bool:
        """
        Set the coefficients for the filter.

        :param coefficients: Coefficients for the filter.
        :return: True if the coefficients were set successfully, False otherwise.
        """
        if coefficients.a0 == 0.0:
            return False
        self.coefficients = coefficients
        self._normalize_coefficients()
        self.reset()
        return True

    def reset(self):
        """
        Reset the filter state.

        :return: None
        """
        self.state = State()
        self.iter = 0

    def _normalize_coefficients(self):
        """
        Normalize the coefficients to make a0 equal to 1.

        :return: None
        """
        a0 = self.coefficients.a0
        self.coefficients.b0 /= a0
        self.coefficients.b1 /= a0
        self.coefficients.b2 /= a0
        self.coefficients.a1 /= a0
        self.coefficients.a2 /= a0
        self.coefficients.a0 = 1.0
