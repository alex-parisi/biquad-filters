/// NotchFilter.h

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

#ifndef NOTCH_FILTER_H
#define NOTCH_FILTER_H

#include "DigitalBiquadFilter.h"
#include "FilterObject.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

/**
 * @brief Notch Filter
 * @details A notch filter is a filter that passes all frequencies except those
 * in a stop band centered around a center frequency. The notch filter is
 * created by inverting the band pass filter.
 * @see DigitalBiquadFilter
 */
template<std::floating_point T = double>
class NotchFilter final : public FilterObject<T> {
public:
    /**
     * @brief Create a notch filter
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter, default value is
     * 0.7071067811865476, or 1/sqrt(2)
     * @return A notch filter object
     */
    static auto create(T cutoff, int sampleRate,
                       T qFactor = static_cast<T>(0.7071067811865476))
            -> std::optional<NotchFilter> {
        if (!FilterObject<T>::verify_parameters(cutoff, sampleRate, qFactor)) {
            return std::nullopt;
        }
        auto obj = NotchFilter(cutoff, sampleRate, qFactor);
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
    NotchFilter(const T cutoff, const int sampleRate, const T qFactor) {
        this->m_cutoff = cutoff;
        this->m_sampleRate = sampleRate;
        this->m_qFactor = qFactor;
        Coefficients<T> coefficients = NotchFilter::calculate_coefficients();
        this->m_filter = DigitalBiquadFilter<T>::create(coefficients);
    }
    /**
     * @brief Calculate the filter coefficients
     * @details Calculates the filter coefficients based on the cutoff
     * frequency, sample rate, and quality factor
     */
    Coefficients<T> calculate_coefficients() override {
        const T w0 = static_cast<T>(2.0 * M_PI * this->m_cutoff /
                                    this->m_sampleRate);
        const T cos_w0 = std::cos(w0);
        const T alpha = std::sin(w0) / (static_cast<T>(2.0) * this->m_qFactor);
        const T b0 = static_cast<T>(1.0);
        const T b1 = static_cast<T>(-2.0) * cos_w0;
        const T b2 = static_cast<T>(1.0);
        const T a0 = static_cast<T>(1.0) + alpha;
        const T a1 = static_cast<T>(-2.0) * cos_w0;
        const T a2 = static_cast<T>(1.0) - alpha;
        return Coefficients{b0, b1, b2, a0, a1, a2};
    }
};

#endif // NOTCH_FILTER_H
