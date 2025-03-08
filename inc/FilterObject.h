/// FilterObject.h

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

#ifndef FILTER_OBJECT_H
#define FILTER_OBJECT_H

#include <cstddef>
#include <optional>

#include "DigitalBiquadFilter.h"

/**
 * @brief Filter Object abstract class template
 * @tparam T floating point type (float/double)
 * @tparam blockSize The block size of the filter (optional)
 */
template<std::floating_point T, size_t blockSize = 0>
class FilterObject {
public:
    virtual ~FilterObject() = default;

    /**
     * @brief Process a sample
     * @details Processes a single sample of audio data
     * @param sample The input sample
     * @return The processed sample
     */
    virtual auto process(const T &sample) -> std::optional<T> {
        if (m_filter.has_value()) {
            return m_filter->process(sample);
        }
        return std::nullopt;
    }
    /**
     * @brief Process a block of samples
     * @details Processes a block of samples of audio data
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place. This assumes that the pointer has enough memory allocated to
     * pull out blockSize samples and is inherently unsafe. For block,
     * processing, it's safer to use the block process methods with either a
     * std::vector or std::array.
     * @return True if the samples were processed successfully, false otherwise
     */
    virtual auto process(T *samples) -> bool {
        if (m_filter.has_value()) {
            m_filter->process(samples);
            return true;
        }
        return false;
    }
    /**
     * @brief Process a block of samples
     * @details Processes a block of samples of audio data
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place. This assumes that the pointer has enough memory allocated to
     * pull out count samples and is inherently unsafe. For block,
     * processing, it's safer to use the block process methods with either a
     * std::vector or std::array.
     * @param count The number of samples to process
     * @return True if the samples were processed successfully, false otherwise
     */
    virtual auto process(T *samples, size_t count) -> bool {
        if (m_filter.has_value()) {
            m_filter->process(samples, count);
            return true;
        }
        return false;
    }
    /**
     * @brief Process a block of samples
     * @details Processes a block of samples of audio data. Since this method
     * uses a std::vector as input, we don't need to rely on blockSize being
     * passed in. The samples are edited in place.
     * @param samples A vector of samples
     * @return True if the samples were processed successfully, false otherwise
     */
    virtual auto process(std::vector<T> &samples) -> bool {
        if (m_filter.has_value()) {
            m_filter->process(samples);
            return true;
        }
        return false;
    }
    /**
     * @brief Process a block of samples
     * @details Processes a block of samples of audio data. The samples are
     * processed in-place.
     * @param samples An array of samples
     * @return True if the samples were processed successfully, false otherwise
     */
    virtual auto process(std::array<T, blockSize> &samples) -> bool {
        if (m_filter.has_value()) {
            m_filter->process(samples);
            return true;
        }
        return false;
    }
    /**
     * @brief Set the cutoff frequency of the filter
     * @param cutoff The cutoff frequency of the filter
     * @return True if the cutoff frequency was set successfully, false
     * otherwise
     */
    virtual auto set_cutoff(const double cutoff) -> bool {
        if (cutoff <= 0) {
            return false;
        }
        m_cutoff = cutoff;
        Coefficients<T> newCoefficients = calculate_coefficients();
        if (m_filter.has_value()) {
            m_filter->set_coefficients(newCoefficients);
            return true;
        }
        return false;
    }
    /**
     * @brief Get the cutoff frequency of the filter
     * @return The cutoff frequency of the filter
     */
    [[nodiscard]] virtual auto get_cutoff() const -> double { return m_cutoff; }
    /**
     * @brief Set the sample rate of the input signal
     * @param sampleRate The sample rate of the input signal
     * @return True if the sample rate was set successfully, false otherwise
     */
    virtual auto set_sample_rate(const int sampleRate) -> bool {
        if (sampleRate <= 0) {
            return false;
        }
        m_sampleRate = sampleRate;
        Coefficients<T> newCoefficients = calculate_coefficients();
        if (m_filter.has_value()) {
            m_filter->set_coefficients(newCoefficients);
            return true;
        }
        return false;
    }
    /**
     * @brief Get the sample rate of the input signal
     * @return The sample rate of the input signal
     */
    [[nodiscard]] virtual auto get_sample_rate() const -> int {
        return m_sampleRate;
    }
    /**
     * @brief Set the quality factor of the filter
     * @param qFactor The quality factor of the filter
     * @return True if the quality factor was set successfully, false otherwise
     */
    virtual auto set_q_factor(const double qFactor) -> bool {
        if (qFactor <= 0) {
            return false;
        }
        m_qFactor = qFactor;
        Coefficients<T> newCoefficients = calculate_coefficients();
        if (m_filter.has_value()) {
            m_filter->set_coefficients(newCoefficients);
            return true;
        }
        return false;
    }
    /**
     * @brief Get the quality factor of the filter
     * @return The quality factor of the filter
     */
    [[nodiscard]] virtual auto get_q_factor() const -> double {
        return m_qFactor;
    }
    /**
     * @brief Set the bandwidth of the filter
     * @param bandwidth The bandwidth of the filter
     * @return True if the bandwidth was set successfully, false otherwise
     */
    virtual auto set_bandwidth(const float bandwidth) -> bool {
        if (bandwidth <= 0) {
            return false;
        }
        const double Q =
                1.0 / (2.0 * std::sinh(bandwidth * std::log10(2.0) / 2.0));
        return set_q_factor(Q);
    }
    /**
     * @brief Get the bandwidth of the filter
     * @return The bandwidth of the filter
     */
    [[nodiscard]] virtual auto get_bandwidth() const -> double {
        const double bandwidth =
                2.0 * std::asinh(1.0 / (2.0 * m_qFactor)) / std::log10(2.0);
        return bandwidth;
    }
    /**
     * @brief Set the gain of the filter
     * @param gain The gain of the filter
     * @return True if the gain was set successfully, false otherwise
     */
    virtual auto set_gain(const double gain) -> bool {
        m_gain = gain;
        Coefficients<T> newCoefficients = calculate_coefficients();
        if (m_filter.has_value()) {
            m_filter->set_coefficients(newCoefficients);
            return true;
        }
        return false;
    }
    /**
     * @brief Get the gain of the filter
     * @return The gain of the filter
     */
    [[nodiscard]] virtual auto get_gain() const -> double { return m_gain; }
    /**
     * @brief Set the constant skirt gain of the filter
     * @param constantSkirtGain Whether to use a constant skirt gain or not
     */
    virtual auto set_constant_skirt_gain(const bool constantSkirtGain) -> bool {
        m_constantSkirtGain = constantSkirtGain;
        Coefficients<T> newCoefficients = calculate_coefficients();
        if (m_filter.has_value()) {
            m_filter->set_coefficients(newCoefficients);
            return true;
        }
        return false;
    }
    /**
     * @brief Get the constant skirt gain of the filter
     * @return Whether the filter uses a constant skirt gain or not
     */
    [[nodiscard]] virtual auto get_constant_skirt_gain() const -> bool {
        return m_constantSkirtGain;
    }

protected:
    /**
     * @brief Calculate the filter coefficients
     * @return The filter coefficients
     * To be implemented by the derived classes
     */
    virtual auto calculate_coefficients() -> Coefficients<T> = 0;
    /**
     * @brief Verify the filter parameters
     * @param cutoff The cutoff frequency of the filter
     * @param sampleRate The sample rate of the input signal
     * @param qFactor The quality factor of the filter
     * @return
     */
    static auto verify_parameters(const double cutoff, const int sampleRate,
                                  const double qFactor) -> bool {
        if (sampleRate <= 0 || cutoff <= 0.0 || qFactor <= 0.0 ||
            cutoff > sampleRate / 2.0) {
            return false;
        }
        return true;
    }
    /** The underlying digital biquad filter */
    std::optional<DigitalBiquadFilter<T, blockSize>> m_filter = std::nullopt;
    /** The cutoff frequency of the filter */
    double m_cutoff = 0.0;
    /** The sample rate of the input signal */
    int m_sampleRate = 0;
    /** The quality factor of the filter */
    double m_qFactor = 0.0;
    /** The gain of the filter */
    double m_gain = 0.0;
    /** Whether to use a constant skirt gain or not */
    bool m_constantSkirtGain = false;
};

#endif // FILTER_OBJECT_H
