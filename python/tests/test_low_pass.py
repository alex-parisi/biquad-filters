# test_low_pass.py

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
import math

from biquads import LowPassFilter


class TestLowPassFilter(unittest.TestCase):
    """
    Test the LowPassFilter class.
    """

    def test_create_valid_double_filter(self):
        """
        Test the creation of a valid filter object with a double cutoff frequency.
        """
        lpf = LowPassFilter.create(1000.0, 44100)
        self.assertIsNotNone(lpf)

    def test_create_valid_float_filter(self):
        """
        Test the creation of a valid filter object with a float cutoff frequency.
        """
        lpf = LowPassFilter.create(1000.0, 44100)
        self.assertIsNotNone(lpf)

    def test_create_invalid_double_filter(self):
        """
        Test the creation of an invalid filter object with a double cutoff frequency.
        """
        lpf = LowPassFilter.create(1000.0, 0)
        self.assertIsNone(lpf)

    def test_create_invalid_float_filter(self):
        """
        Test the creation of an invalid filter object with a float cutoff frequency.
        """
        lpf = LowPassFilter.create(1000.0, 0)
        self.assertIsNone(lpf)

    def test_set_cutoff_frequency(self):
        """
        Test the set_cutoff method.
        """
        lpf = LowPassFilter.create(1000.0, 44100)
        self.assertIsNotNone(lpf)
        self.assertAlmostEqual(lpf.get_cutoff(), 1000.0)
        lpf.set_cutoff(2000.0)
        self.assertAlmostEqual(lpf.get_cutoff(), 2000.0)

    def test_set_sample_rate(self):
        """
        Test the set_sample_rate method.
        """
        lpf = LowPassFilter.create(1000.0, 44100)
        self.assertIsNotNone(lpf)
        self.assertEqual(lpf.get_sample_rate(), 44100)
        lpf.set_sample_rate(48000)
        self.assertEqual(lpf.get_sample_rate(), 48000)

    def test_set_quality_factor(self):
        """
        Test the set_q_factor method.
        """
        lpf = LowPassFilter.create(1000.0, 44100)
        self.assertIsNotNone(lpf)
        self.assertAlmostEqual(lpf.get_q_factor(), 1 / math.sqrt(2))
        lpf.set_q_factor(1.0)
        self.assertAlmostEqual(lpf.get_q_factor(), 1.0)


if __name__ == "__main__":
    unittest.main()
