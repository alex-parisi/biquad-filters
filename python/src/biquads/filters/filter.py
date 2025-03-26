# filter.py

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

import numpy as np
from typing import Optional

from src.biquads.filters.biquad import DigitalBiquadFilter, Coefficients


class FilterObject:
    """
    Base class for a filter object.
    """

    def __init__(self):
        """
        Initialize the filter object.
        """
        self.m_filter: Optional[DigitalBiquadFilter] = None
        self.m_cutoff: float = 0.0
        self.m_sampleRate: int = 0
        self.m_qFactor: float = 0.0
        self.m_gain: float = 0.0
        self.m_constantSkirtGain: bool = False
        self.m_bypass: bool = False

    def process(self, sample: float) -> float:
        """
        Process a single sample through the filter.

        :param sample: Input sample.
        :return: Filtered output.
        """
        if self.m_bypass or not self.m_filter:
            return sample
        return self.m_filter.process(sample)

    def process_block(self, samples: np.ndarray) -> np.ndarray:
        """
        Process a block of samples through the filter.

        :param samples: Input samples.
        :return: Filtered output.
        """
        if self.m_bypass or not self.m_filter:
            return samples
        return self.m_filter.process_block(samples)

    def set_cutoff(self, cutoff: float) -> bool:
        """
        Set the cutoff frequency of the filter.

        :param cutoff: Cutoff frequency.
        :return: True if the cutoff frequency was set successfully, False otherwise.
        """
        if cutoff <= 0.0:
            return False
        self.m_cutoff = cutoff
        return self._update_coefficients()

    def get_cutoff(self) -> float:
        """
        Get the cutoff frequency of the filter.

        :return: Cutoff frequency.
        """
        return self.m_cutoff

    def set_sample_rate(self, sample_rate: int) -> bool:
        """
        Set the sample rate of the filter.

        :param sample_rate: Sample rate.
        :return: True if the sample rate was set successfully, False otherwise.
        """
        if sample_rate <= 0:
            return False
        self.m_sampleRate = sample_rate
        return self._update_coefficients()

    def get_sample_rate(self) -> int:
        """
        Get the sample rate of the filter.

        :return: Sample rate.
        """
        return self.m_sampleRate

    def set_q_factor(self, q_factor: float) -> bool:
        """
        Set the Q factor of the filter.

        :param q_factor: Q factor.
        :return: True if the Q factor was set successfully, False otherwise.
        """
        if q_factor <= 0.0:
            return False
        self.m_qFactor = q_factor
        return self._update_coefficients()

    def get_q_factor(self) -> float:
        """
        Get the Q factor of the filter.

        :return: Q factor.
        """
        return self.m_qFactor

    def set_bypass(self, bypass: bool) -> None:
        """
        Set the bypass state of the filter.

        :param bypass: Bypass state.
        :return: None
        """
        self.m_bypass = bypass

    def get_bypass(self) -> bool:
        """
        Get the bypass state of the filter.

        :return: Bypass state.
        """
        return self.m_bypass

    def _update_coefficients(self) -> bool:
        """
        Update the filter coefficients.

        :return: True if the coefficients were updated successfully, False otherwise.
        """
        if not self.verify_parameters(self.m_cutoff, self.m_sampleRate, self.m_qFactor):
            return False
        coefficients = self.calculate_coefficients()
        if self.m_filter:
            return self.m_filter.set_coefficients(coefficients)
        return False

    def calculate_coefficients(self) -> Coefficients:
        """
        Calculate the filter coefficients.
        :return: Coefficients for the filter.
        """
        raise NotImplementedError("Must be implemented in derived class")

    @staticmethod
    def verify_parameters(cutoff: float, sample_rate: int, q: float) -> bool:
        """
        Verify the filter parameters.

        :param cutoff: The cutoff frequency.
        :param sample_rate: The sample rate.
        :param q: The Q factor.
        :return: True if the parameters are valid, False otherwise.
        """
        return 0.0 < cutoff < (sample_rate / 2) and sample_rate > 0 and q > 0.0
