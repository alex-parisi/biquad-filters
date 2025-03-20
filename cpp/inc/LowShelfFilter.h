/// LowShelfFilter.h

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

#ifndef LOW_SHELF_FILTER_H
#define LOW_SHELF_FILTER_H

#include "DigitalBiquadFilter.h"
#include "FilterObject.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

/**
 * @brief Low Shelf Filter
 * @details A low shelf filter is a type of peaking EQ filter that boosts or
 * attenuates frequencies below the cutoff frequency while leaving higher
 * frequencies unaffected. The filter has a peak at the cutoff frequency and
 * the gain is defined in decibels.
 * @see DigitalBiquadFilter
 */
template<std::floating_point T = double>
class LowShelfFilter final : public FilterObject<T> {
public:
    /**
     * @brief Create a low shelf filter
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter, default value is
     * 0.7071067811865476, or 1/sqrt(2)
     * @param gain The gain of the filter in decibels, default value is 6.0
     * @return A low shelf filter object
     */
    static auto create(T cutoff, int sampleRate, T qFactor = 0.7071067811865476,
                       T gain = 6.0) -> std::optional<LowShelfFilter> {
        if (!FilterObject<T>::verify_parameters(cutoff, sampleRate, qFactor)) {
            return std::nullopt;
        }
        auto obj = LowShelfFilter(cutoff, sampleRate, qFactor, gain);
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
     * @param gain The gain of the filter in decibels
     */
    LowShelfFilter(const T cutoff, const int sampleRate, const T qFactor,
                   const T gain) {
        this->m_cutoff = cutoff;
        this->m_sampleRate = sampleRate;
        this->m_qFactor = qFactor;
        this->m_gain = gain;
        Coefficients<T> coefficients = LowShelfFilter::calculate_coefficients();
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
        const T A = std::pow(10.0, this->m_gain / 40.0);
        const T b0 =
                A * (A + 1.0 - (A - 1.0) * cos_w0 + 2.0 * std::sqrt(A) * alpha);
        const T b1 = 2.0 * A * (A - 1.0 - (A + 1.0) * cos_w0);
        const T b2 =
                A * (A + 1.0 - (A - 1.0) * cos_w0 - 2.0 * std::sqrt(A) * alpha);
        const T a0 = A + 1.0 + (A - 1.0) * cos_w0 + 2.0 * std::sqrt(A) * alpha;
        const T a1 = -2.0 * (A - 1.0 + (A + 1.0) * cos_w0);
        const T a2 = A + 1.0 + (A - 1.0) * cos_w0 - 2.0 * std::sqrt(A) * alpha;
        return Coefficients{b0, b1, b2, a0, a1, a2};
    }
};

#endif // LOW_SHELF_FILTER_H
