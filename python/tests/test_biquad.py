# test_biquad.py

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

import unittest
import numpy as np

from src.biquads.filters.biquad import Coefficients, DigitalBiquadFilter


class TestDigitalBiquadFilter(unittest.TestCase):
    """
    Test the DigitalBiquadFilter class
    """

    def test_create_valid_double_filter(self):
        """
        Test the creation of a valid filter object with double coefficients.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        self.assertIsNotNone(dbf)

    def test_create_valid_float_filter(self):
        """
        Test the creation of a valid filter object with float coefficients.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        self.assertIsNotNone(dbf)

    def test_create_invalid_double_filter(self):
        """
        Test the creation of an invalid filter object with double coefficients.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 0.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        self.assertIsNone(dbf)

    def test_process_single_sample_double(self):
        """
        Test the processing of a single sample with double precision.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        sample = 1.0
        self.assertAlmostEqual(dbf.process(sample), 1.0)

    def test_process_single_sample_float(self):
        """
        Test the processing of a single sample with single precision.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        sample = np.float32(1.0)
        self.assertAlmostEqual(dbf.process(sample), 1.0)

    def test_process_block_double(self):
        """
        Test the processing of a block of samples with double
        precision.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        data = np.array([1.0, 0.5, 0.25])
        out = dbf.process_block(data)
        np.testing.assert_array_almost_equal(out, [1.0, 0.5, 0.25])

    def test_process_block_float(self):
        """
        Test the processing of a block of samples with single
        precision.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        data = np.array([1.0, 0.5, 0.25], dtype=np.float32)
        out = dbf.process_block(data)
        np.testing.assert_array_almost_equal(out, [1.0, 0.5, 0.25])

    def test_reset(self):
        """
        Test the reset method.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        sample = 1.0
        dbf.process(sample)
        dbf.reset()
        sample2 = 1.0
        self.assertAlmostEqual(dbf.process(sample2), 1.0)

    def test_zero_coefficients(self):
        """
        Test the processing of a single sample with zero coefficients.
        """
        coefficients = Coefficients(0.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        sample = 1.0
        self.assertAlmostEqual(dbf.process(sample), 0.0)

    def test_block_of_zeros(self):
        """
        Test the processing of a block of zeros.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        samples = np.zeros(5)
        out = dbf.process_block(samples)
        np.testing.assert_array_almost_equal(out, np.zeros(5))

    def test_ramp_signal(self):
        """
        Test the processing of a ramp signal.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        ramp = np.array([0.0, 1.0, 2.0, 3.0, 4.0])
        out = dbf.process_block(ramp)
        np.testing.assert_array_almost_equal(out, ramp)

    def test_large_value_stability(self):
        """
        Test the stability of the filter with a large input value.
        """
        coefficients = Coefficients(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        large = 1e6
        self.assertAlmostEqual(dbf.process(large), large)

    def test_negative_coefficients(self):
        """
        Test the processing of a single sample with negative coefficients.
        """
        coefficients = Coefficients(-1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
        dbf = DigitalBiquadFilter.create(coefficients)
        sample = 1.0
        self.assertAlmostEqual(dbf.process(sample), -1.0)


if __name__ == "__main__":
    unittest.main()
