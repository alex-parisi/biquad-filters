/// DigitalBiquadFilter.h

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

#ifndef DIGITAL_BIQUAD_FILTER_H
#define DIGITAL_BIQUAD_FILTER_H

#include <optional>

/**
 * @brief Coefficients Struct
 * @details A struct containing the coefficients of a digital biquad filter
 */
template<std::floating_point T = double>
struct Coefficients {
    T b0, b1, b2, a0, a1, a2;
};

/**
 * @brief State Struct
 * @details A struct containing the state variables of a digital biquad filter
 */
template<std::floating_point T = double>
struct State {
    T x1, x2, y1, y2;
};

/**
 * @brief Digital Biquad Filter
 * @details A digital biquad filter is a type of infinite impulse response
 * filter. The transfer function is defined by the following equation: H(z) =
 * (b0 + b1*z^-1 + b2*z^-2) / (a0 + a1*z^-1 + a2*z^-2) The filter is implemented
 * by applying the following difference equation: y[n] = b0*x[n] + b1*x[n-1] +
 * b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
 */
template<std::floating_point T = double>
class DigitalBiquadFilter {
public:
    /**
     * @brief Public Constructor
     * @details Creates a filter with specified coefficients
     * @param coefficients A Coefficients struct containing the filter
     * coefficients: b0, b1 b2, a0, a1, a2
     * @see Coefficients
     */
    static auto create(const Coefficients<T> &coefficients)
            -> std::optional<DigitalBiquadFilter> {
        if (coefficients.a0 == 0.0) {
            return std::nullopt;
        }
        return DigitalBiquadFilter(coefficients);
    }
    /**
     * @brief Process a sample
     * @details Processes a single sample of audio data in-place
     * @param sample The input sample
     */
    auto process(T &sample) -> void {
        const T output =
                ((m_coefficients.b0 / m_coefficients.a0) * sample) +
                ((m_coefficients.b1 / m_coefficients.a0) * m_state.x1) +
                ((m_coefficients.b2 / m_coefficients.a0) * m_state.x2) -
                ((m_coefficients.a1 / m_coefficients.a0) * m_state.y1) -
                ((m_coefficients.a2 / m_coefficients.a0) * m_state.y2);
        m_state.x2 = m_state.x1;
        m_state.x1 = sample;
        m_state.y2 = m_state.y1;
        m_state.y1 = output;
        sample = output;

        m_iter++;
    }
    /**
     * @brief Process a block of samples
     * @details Processes a block of samples of audio data
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place.
     * @param count The number of samples in the block
     * This assumes that the pointer has enough memory allocated to pull out
     * count samples.
     * @return True if the samples were processed successfully, false otherwise
     */
    auto process(T *samples, const size_t count) -> bool {
        if (samples == nullptr || count <= 0) {
            return false;
        }
        for (size_t i = 0; i < count; ++i) {
            process(samples[i]);
        }
        return true;
    }
    /**
     * @brief Set the filter coefficients
     * @details Sets the filter coefficients to custom values
     * @param coefficients A Coefficients struct containing the filter
     * coefficients: b0, b1 b2, a0, a1, a2
     * @see Coefficients
     * @return True if the coefficients were set successfully, false otherwise
     */
    auto set_coefficients(const Coefficients<T> &coefficients) -> bool {
        if (coefficients.a0 == 0.0) {
            return false;
        }
        m_coefficients = coefficients;
        reset();
        return true;
    }
    /**
     * @brief Reset the filter
     * @details Resets the filter state variables
     */
    auto reset() -> void {
        m_state.x1 = 0.0;
        m_state.x2 = 0.0;
        m_state.y1 = 0.0;
        m_state.y2 = 0.0;
        m_iter = 0;
    }

private:
    /**
     * @brief Private Constructor
     * @details Initializes the filter with specified coefficients
     * @param coefficients A Coefficients struct containing the filter
     * coefficients: b0, b1 b2, a0, a1, a2
     * @see Coefficients
     */
    explicit DigitalBiquadFilter(const Coefficients<T> &coefficients) :
        m_coefficients(coefficients) {}

    /** The filter coefficients */
    Coefficients<T> m_coefficients = {};

    /** The filter state variables */
    State<T> m_state = {};

    /** The number of iterations */
    long long m_iter = 0LL;
};

#endif // DIGITAL_BIQUAD_FILTER_H
