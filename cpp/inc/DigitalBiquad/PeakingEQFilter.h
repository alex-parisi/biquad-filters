/// PeakingEQFilter.h

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

#ifndef PEAKING_EQ_FILTER_H
#define PEAKING_EQ_FILTER_H

#include "DigitalBiquadFilter.h"
#include "FilterObject.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

/**
 * @brief Peaking EQ Filter
 * @details A peaking EQ filter is a signal processing filter that passes all
 * frequencies equally in gain, but boosts or attenuates a specific range of
 * frequencies. This filter is useful for creating a parametric equalizer.
 * @see DigitalBiquadFilter
 */
template<std::floating_point T = double>
class PeakingEQFilter final : public FilterObject<T> {
public:
    /**
     * @brief Create a peaking EQ filter
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter, default value is
     * 0.7071067811865476, or 1/sqrt(2)
     * @param gain The gain of the filter in decibels, default value is 6.0
     * @return A peaking EQ filter object
     */
    static auto create(T cutoff, int sampleRate,
                       T qFactor = static_cast<T>(0.7071067811865476),
                       T gain = static_cast<T>(6.0))
            -> std::optional<PeakingEQFilter> {
        if (!FilterObject<T>::verify_parameters(cutoff, sampleRate, qFactor)) {
            return std::nullopt;
        }
        auto obj = PeakingEQFilter(cutoff, sampleRate, qFactor, gain);
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
    PeakingEQFilter(const T cutoff, const int sampleRate, const T qFactor,
                    const T gain) {
        this->m_cutoff = cutoff;
        this->m_sampleRate = sampleRate;
        this->m_qFactor = qFactor;
        this->m_gain = gain;
        Coefficients<T> coefficients =
                PeakingEQFilter::calculate_coefficients();
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
        const T A = std::pow(static_cast<T>(10.0),
                             this->m_gain / static_cast<T>(40.0));
        const T b0 = static_cast<T>(1.0) + alpha * A;
        const T b1 = static_cast<T>(-2.0) * cos_w0;
        const T b2 = static_cast<T>(1.0) - alpha * A;
        const T a0 = static_cast<T>(1.0) + alpha / A;
        const T a1 = static_cast<T>(-2.0) * cos_w0;
        const T a2 = static_cast<T>(1.0) - alpha / A;
        return Coefficients{b0, b1, b2, a0, a1, a2};
    }
};

#endif // PEAKING_EQ_FILTER_H
