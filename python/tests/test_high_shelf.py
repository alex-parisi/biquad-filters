# test_high_shelf.py

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

from biquads import HighShelfFilter


class TestHighShelfFilter(unittest.TestCase):
    """
    Test the HighShelfFilter class.
    """

    def test_create_valid_double_filter(self):
        """
        Test the creation of a valid filter object with a double cutoff frequency.
        """
        hsf = HighShelfFilter.create(1000.0, 44100)
        self.assertIsNotNone(hsf)

    def test_create_valid_float_filter(self):
        """
        Test the creation of a valid filter object with a float cutoff frequency.
        """
        hsf = HighShelfFilter.create(1000.0, 44100)
        self.assertIsNotNone(hsf)

    def test_create_invalid_double_filter(self):
        """
        Test the creation of an invalid filter object with a double cutoff frequency.
        """
        hsf = HighShelfFilter.create(1000.0, 0)
        self.assertIsNone(hsf)

    def test_create_invalid_float_filter(self):
        """
        Test the creation of an invalid filter object with a float cutoff frequency.
        """
        hsf = HighShelfFilter.create(1000.0, 0)
        self.assertIsNone(hsf)

    def test_set_cutoff_frequency(self):
        """
        Test the set_cutoff method.
        """
        hsf = HighShelfFilter.create(1000.0, 44100)
        self.assertIsNotNone(hsf)
        self.assertAlmostEqual(hsf.get_cutoff(), 1000.0)
        hsf.set_cutoff(2000.0)
        self.assertAlmostEqual(hsf.get_cutoff(), 2000.0)

    def test_set_sample_rate(self):
        """
        Test the set_sample_rate method.
        """
        hsf = HighShelfFilter.create(1000.0, 44100)
        self.assertIsNotNone(hsf)
        self.assertEqual(hsf.get_sample_rate(), 44100)
        hsf.set_sample_rate(48000)
        self.assertEqual(hsf.get_sample_rate(), 48000)

    def test_set_quality_factor(self):
        """
        Test the set_q_factor method.
        """
        hsf = HighShelfFilter.create(1000.0, 44100)
        self.assertIsNotNone(hsf)
        self.assertAlmostEqual(hsf.get_q_factor(), 1 / math.sqrt(2))
        hsf.set_q_factor(1.0)
        self.assertAlmostEqual(hsf.get_q_factor(), 1.0)


if __name__ == "__main__":
    unittest.main()
