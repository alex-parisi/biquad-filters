/// LowPassFilter.h

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

#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "DigitalBiquadFilter.h"
#include "FilterObject.h"

/**
 * @brief Low Pass Filter
 * @details A low pass filter is a filter that passes signals with a
 * frequency lower than a certain cutoff frequency and attenuates signals
 * with frequencies higher than the cutoff frequency.
 * @see DigitalBiquadFilter
 */
template<std::floating_point T, size_t blockSize = 0>
class LowPassFilter final : public FilterObject<T, blockSize> {
public:
    /**
     * @brief Create a low pass filter
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter, default value is
     * 0.7071067811865476, or 1/sqrt(2)
     * @return A low pass filter object
     */
    static auto create(double cutoff, int sampleRate,
                       double qFactor = 0.7071067811865476)
            -> std::optional<LowPassFilter> {
        if (!FilterObject<T, blockSize>::verify_parameters(cutoff, sampleRate,
                                                           qFactor)) {
            return std::nullopt;
        }
        auto obj = LowPassFilter(cutoff, sampleRate, qFactor);
        if (!obj.m_filter.has_value()) {
            return std::nullopt;
        }
        return obj;
    }

private:
    /**
     * @brief Private Constructor
     * @details Initializes the filter with parameters
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter
     */
    LowPassFilter(const double cutoff, const int sampleRate,
                  const double qFactor) {
        this->m_cutoff = cutoff;
        this->m_sampleRate = sampleRate;
        this->m_qFactor = qFactor;
        Coefficients<T> coefficients = LowPassFilter::calculate_coefficients();
        this->m_filter =
                DigitalBiquadFilter<T, blockSize>::create(coefficients);
    }
    /**
     * @brief Calculate the filter coefficients
     * @details Calculates the filter coefficients based on the cutoff
     * frequency, sample rate, and quality factor
     */
    Coefficients<T> calculate_coefficients() override {
        const T w0 = 2.0 * M_PI * this->m_cutoff / this->m_sampleRate;
        const T cos_w0 = std::cos(w0);
        const T alpha = std::sin(w0) / (2.0 * this->m_qFactor);
        const T b1 = 1.0 - cos_w0;
        const T b0 = b1 / 2.0;
        const T b2 = b0;
        const T a0 = 1.0 + alpha;
        const T a1 = -2.0 * cos_w0;
        const T a2 = 1.0 - alpha;
        return Coefficients{b0, b1, b2, a0, a1, a2};
    }
};

#endif // LOW_PASS_FILTER_H
