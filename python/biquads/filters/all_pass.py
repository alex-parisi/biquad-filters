# all_pass.py

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
import math
from typing import Optional

from biquads.filters.biquad import DigitalBiquadFilter, Coefficients
from biquads.filters.filter import FilterObject


class AllPassFilter(FilterObject):
    """
    All-pass filter object.
    """

    def __init__(self, cutoff: float, sample_rate: int, q_factor: float = 1.0 / math.sqrt(2.0)):
        """
        Initialize the all-pass filter object.
        :param cutoff: The center frequency.
        :param sample_rate: The sample rate.
        :param q_factor: The Q factor.
        """
        super().__init__()
        self.m_cutoff = cutoff
        self.m_sampleRate = sample_rate
        self.m_qFactor = q_factor
        coefficients = self.calculate_coefficients()
        self.m_filter = DigitalBiquadFilter.create(coefficients)

    @staticmethod
    def create(cutoff: float, sample_rate: int, q_factor: float = 1.0 / math.sqrt(2.0)) -> Optional['AllPassFilter']:
        """
        Create a all-pass filter object.
        :param cutoff: The center frequency.
        :param sample_rate: The sample rate.
        :param q_factor: The Q factor.
        :return: The all-pass filter object.
        """
        if not FilterObject.verify_parameters(cutoff, sample_rate, q_factor):
            return None
        f = AllPassFilter(cutoff, sample_rate, q_factor)
        if not f.m_filter:
            return None
        return f

    def calculate_coefficients(self) -> Coefficients:
        """
        Calculate the filter coefficients.
        :return: The filter coefficients.
        """
        w0 = 2.0 * math.pi * self.m_cutoff / self.m_sampleRate
        cos_w0 = math.cos(w0)
        alpha = math.sin(w0) / (2.0 * self.m_qFactor)
        b0 = 1.0 - alpha
        b1 = -2.0 * cos_w0
        b2 = 1.0 + alpha
        a0 = 1.0 + alpha
        a1 = -2.0 * cos_w0
        a2 = 1.0 - alpha
        return Coefficients(b0, b1, b2, a0, a1, a2)
