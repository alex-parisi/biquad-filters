/// BandPassFilter.h

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

#ifndef BAND_PASS_FILTER_H
#define BAND_PASS_FILTER_H

#include "DigitalBiquadFilter.h"
#include "FilterObject.h"

/**
 * @brief Band Pass Filter
 * @details A band pass filter is a filter that passes signals with a
 * frequency within a certain range and attenuates signals with frequencies
 * outside of that range.
 * @see DigitalBiquadFilter
 */
template<std::floating_point T>
class BandPassFilter final : public FilterObject<T> {
public:
    /**
     * @brief Create a band pass filter
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter, default value is
     * 0.7071067811865476, or 1/sqrt(2)
     * @param constantSkirtGain Whether to use a constant skirt gain or not
     * @return A band pass filter object
     */
    static auto create(double cutoff, int sampleRate,
                       double qFactor = 0.7071067811865476,
                       bool constantSkirtGain = false)
            -> std::optional<BandPassFilter> {
        if (!FilterObject<T>::verify_parameters(cutoff, sampleRate, qFactor)) {
            return std::nullopt;
        }
        auto obj =
                BandPassFilter(cutoff, sampleRate, qFactor, constantSkirtGain);
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
     * @param constantSkirtGain Whether to use a constant skirt gain or not
     */
    BandPassFilter(const double cutoff, const int sampleRate,
                   const double qFactor, const bool constantSkirtGain) {
        this->m_cutoff = cutoff;
        this->m_sampleRate = sampleRate;
        this->m_qFactor = qFactor;
        this->m_constantSkirtGain = constantSkirtGain;
        Coefficients<T> coefficients = BandPassFilter::calculate_coefficients();
        this->m_filter = DigitalBiquadFilter<T>::create(coefficients);
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
        T b0, b1, b2;
        if (this->m_constantSkirtGain) {
            b0 = this->m_qFactor * alpha;
            b1 = 0;
            b2 = -this->m_qFactor * alpha;
        } else {
            b0 = alpha;
            b1 = 0;
            b2 = -alpha;
        }
        const T a0 = 1.0 + alpha;
        const T a1 = -2.0 * cos_w0;
        const T a2 = 1.0 - alpha;
        return Coefficients{b0, b1, b2, a0, a1, a2};
    }
};

#endif // BAND_PASS_FILTER_H
