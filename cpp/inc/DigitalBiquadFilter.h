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

#if defined(__AVX__)
#include <immintrin.h>
#elif defined(__SSE4_1__)
#include <xmmintrin.h>
#elif defined(__ARM_NEON)
#include <arm_neon.h>
#endif
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
    using value_type = T;
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
        T x0 = sample;
        T x1 = m_state.x1;
        T x2 = m_state.x2;
        T y1 = m_state.y1;
        T y2 = m_state.y2;

        T output = (m_coefficients.b0 * x0) + (m_coefficients.b1 * x1) +
                   (m_coefficients.b2 * x2) - (m_coefficients.a1 * y1) -
                   (m_coefficients.a2 * y2);

        m_state.x2 = x1;
        m_state.x1 = x0;
        m_state.y2 = y1;
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
        /// Detect CPU and use the best available implementation
#if defined(__AVX__)
        process_block_avx(samples, count);
#elif defined(__SSE4_1__)
        process_block_sse(samples, count);
#elif defined(__ARM_NEON)
        process_block_neon(samples, count);
#else
        process_block_scalar(samples, count);
#endif
        return true;
    }
    /**
     * @brief Scalar version of the process method
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place.
     * @param count The number of samples in the block
     */
    void process_block_scalar(T *samples, const size_t count) {
        if (!samples || count <= 0) {
            return;
        }
        for (size_t i = 0; i < count; ++i) {
            process(samples[i]);
        }
    }
#if defined(__AVX__)
    /**
     * @brief Process a block of samples using AVX instructions. This method
     * is faster than the scalar version for large blocks of samples.
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place.
     * @param count The number of samples in the block
     */
    void process_block_avx(T *samples, size_t count) {
        if (!samples) return;
        if constexpr (std::is_same_v<T, float>) {
            if (count < 8) {
                process_block_scalar(samples, count);
                return;
            }
            const __m256 b0 = _mm256_set1_ps(m_coefficients.b0);
            const __m256 b1 = _mm256_set1_ps(m_coefficients.b1);
            const __m256 b2 = _mm256_set1_ps(m_coefficients.b2);
            const __m256 a1 = _mm256_set1_ps(m_coefficients.a1);
            const __m256 a2 = _mm256_set1_ps(m_coefficients.a2);
            __m256 x1 = _mm256_set1_ps(m_state.x1);
            __m256 x2 = _mm256_set1_ps(m_state.x2);
            __m256 y1 = _mm256_set1_ps(m_state.y1);
            __m256 y2 = _mm256_set1_ps(m_state.y2);
            for (size_t i = 0; i + 7 < count; i += 8) {
                const __m256 x0 = _mm256_loadu_ps(samples + i);
                __m256 y0 = _mm256_add_ps(
                    _mm256_add_ps(_mm256_mul_ps(b0, x0), _mm256_mul_ps(b1, x1)),
                    _mm256_mul_ps(b2, x2));
                y0 = _mm256_sub_ps(y0,
                      _mm256_add_ps(_mm256_mul_ps(a1, y1), _mm256_mul_ps(a2, y2)));
                _mm256_storeu_ps(samples + i, y0);
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
            m_state.x1 = _mm256_cvtss_f32(x1);
            m_state.x2 = _mm256_cvtss_f32(x2);
            m_state.y1 = _mm256_cvtss_f32(y1);
            m_state.y2 = _mm256_cvtss_f32(y2);
        } else if constexpr (std::is_same_v<T, double>) {
            if (count < 4) {
                process_block_scalar(samples, count);
                return;
            }
            const __m256d b0 = _mm256_set1_pd(m_coefficients.b0);
            const __m256d b1 = _mm256_set1_pd(m_coefficients.b1);
            const __m256d b2 = _mm256_set1_pd(m_coefficients.b2);
            const __m256d a1 = _mm256_set1_pd(m_coefficients.a1);
            const __m256d a2 = _mm256_set1_pd(m_coefficients.a2);
            __m256d x1 = _mm256_set1_pd(m_state.x1);
            __m256d x2 = _mm256_set1_pd(m_state.x2);
            __m256d y1 = _mm256_set1_pd(m_state.y1);
            __m256d y2 = _mm256_set1_pd(m_state.y2);
            for (size_t i = 0; i + 3 < count; i += 4) {
                const __m256d x0 = _mm256_loadu_pd(samples + i);
                __m256d y0 = _mm256_add_pd(
                    _mm256_add_pd(_mm256_mul_pd(b0, x0), _mm256_mul_pd(b1, x1)),
                    _mm256_mul_pd(b2, x2));
                y0 = _mm256_sub_pd(y0,
                      _mm256_add_pd(_mm256_mul_pd(a1, y1), _mm256_mul_pd(a2, y2)));
                _mm256_storeu_pd(samples + i, y0);
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
            m_state.x1 = _mm256_cvtsd_f64(x1);
            m_state.x2 = _mm256_cvtsd_f64(x2);
            m_state.y1 = _mm256_cvtsd_f64(y1);
            m_state.y2 = _mm256_cvtsd_f64(y2);
        }
    }
#endif
    #if defined(__SSE4_1__)
    /**
     * @brief Process a block of samples using SSE instructions. This method
     * is faster than the scalar version for large blocks of samples.
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place.
     * @param count The number of samples in the block
     */
    void process_block_sse(T *samples, size_t count) {
        if (!samples) return;
        if constexpr (std::is_same_v<T, float>) {
            if (count < 4) {
                process_block_scalar(samples, count);
                return;
            }
            const __m128 b0 = _mm_set1_ps(m_coefficients.b0);
            const __m128 b1 = _mm_set1_ps(m_coefficients.b1);
            const __m128 b2 = _mm_set1_ps(m_coefficients.b2);
            const __m128 a1 = _mm_set1_ps(m_coefficients.a1);
            const __m128 a2 = _mm_set1_ps(m_coefficients.a2);
            __m128 x1 = _mm_set1_ps(m_state.x1);
            __m128 x2 = _mm_set1_ps(m_state.x2);
            __m128 y1 = _mm_set1_ps(m_state.y1);
            __m128 y2 = _mm_set1_ps(m_state.y2);
            for (size_t i = 0; i + 3 < count; i += 4) {
                const __m128 x0 = _mm_loadu_ps(samples + i);
                __m128 y0 = _mm_add_ps(
                    _mm_add_ps(_mm_mul_ps(b0, x0), _mm_mul_ps(b1, x1)),
                    _mm_mul_ps(b2, x2));
                y0 = _mm_sub_ps(y0,
                      _mm_add_ps(_mm_mul_ps(a1, y1), _mm_mul_ps(a2, y2)));
                _mm_storeu_ps(samples + i, y0);
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
            m_state.x1 = _mm_cvtss_f32(x1);
            m_state.x2 = _mm_cvtss_f32(x2);
            m_state.y1 = _mm_cvtss_f32(y1);
            m_state.y2 = _mm_cvtss_f32(y2);
        } else if constexpr (std::is_same_v<T, double>) {
            if (count < 2) {
                process_block_scalar(samples, count);
                return;
            }
            const __m128d b0 = _mm_set1_pd(m_coefficients.b0);
            const __m128d b1 = _mm_set1_pd(m_coefficients.b1);
            const __m128d b2 = _mm_set1_pd(m_coefficients.b2);
            const __m128d a1 = _mm_set1_pd(m_coefficients.a1);
            const __m128d a2 = _mm_set1_pd(m_coefficients.a2);
            __m128d x1 = _mm_set1_pd(m_state.x1);
            __m128d x2 = _mm_set1_pd(m_state.x2);
            __m128d y1 = _mm_set1_pd(m_state.y1);
            __m128d y2 = _mm_set1_pd(m_state.y2);
            for (size_t i = 0; i + 1 < count; i += 2) {
                const __m128d x0 = _mm_loadu_pd(samples + i);
                __m128d y0 = _mm_add_pd(
                    _mm_add_pd(_mm_mul_pd(b0, x0), _mm_mul_pd(b1, x1)),
                    _mm_mul_pd(b2, x2));
                y0 = _mm_sub_pd(y0,
                      _mm_add_pd(_mm_mul_pd(a1, y1), _mm_mul_pd(a2, y2)));
                _mm_storeu_pd(samples + i, y0);
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
            m_state.x1 = _mm_cvtsd_f64(x1);
            m_state.x2 = _mm_cvtsd_f64(x2);
            m_state.y1 = _mm_cvtsd_f64(y1);
            m_state.y2 = _mm_cvtsd_f64(y2);
        }
    }
#endif
    #if defined(__ARM_NEON)
    /**
     * @brief Process a block of samples using NEON instructions. This method
     * is faster than the scalar version for large blocks of samples.
     * @param samples A pointer to the block of samples. The samples are edited
     * in-place.
     * @param count The number of samples in the block
     */
    void process_block_neon(T *samples, const size_t count) {
        if (!samples) return;
        if constexpr (std::is_same_v<T, float>) {
            if (count < 4) {
                process_block_scalar(samples, count);
                return;
            }
            const float32x4_t b0 = vdupq_n_f32(m_coefficients.b0);
            const float32x4_t b1 = vdupq_n_f32(m_coefficients.b1);
            const float32x4_t b2 = vdupq_n_f32(m_coefficients.b2);
            const float32x4_t a1 = vdupq_n_f32(m_coefficients.a1);
            const float32x4_t a2 = vdupq_n_f32(m_coefficients.a2);
            float32x4_t x1 = vdupq_n_f32(m_state.x1);
            float32x4_t x2 = vdupq_n_f32(m_state.x2);
            float32x4_t y1 = vdupq_n_f32(m_state.y1);
            float32x4_t y2 = vdupq_n_f32(m_state.y2);
            for (size_t i = 0; i + 3 < count; i += 4) {
                const float32x4_t x0 = vld1q_f32(samples + i);
                float32x4_t y0 = vmlaq_f32(vmlaq_f32(vmulq_f32(b0, x0), b1, x1), b2, x2);
                y0 = vsubq_f32(y0, vmlaq_f32(vmulq_f32(a1, y1), a2, y2));
                vst1q_f32(samples + i, y0);
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
            m_state.x1 = vgetq_lane_f32(x1, 0);
            m_state.x2 = vgetq_lane_f32(x2, 0);
            m_state.y1 = vgetq_lane_f32(y1, 0);
            m_state.y2 = vgetq_lane_f32(y2, 0);
        } else if constexpr (std::is_same_v<T, double>) {
            /// NEON double-precision (float64x2_t) is available on ARMv8-A.
            if (count < 2) {
                process_block_scalar(samples, count);
                return;
            }
            const float64x2_t b0 = vdupq_n_f64(m_coefficients.b0);
            const float64x2_t b1 = vdupq_n_f64(m_coefficients.b1);
            const float64x2_t b2 = vdupq_n_f64(m_coefficients.b2);
            const float64x2_t a1 = vdupq_n_f64(m_coefficients.a1);
            const float64x2_t a2 = vdupq_n_f64(m_coefficients.a2);
            float64x2_t x1 = vdupq_n_f64(m_state.x1);
            float64x2_t x2 = vdupq_n_f64(m_state.x2);
            float64x2_t y1 = vdupq_n_f64(m_state.y1);
            float64x2_t y2 = vdupq_n_f64(m_state.y2);
            for (size_t i = 0; i + 1 < count; i += 2) {
                const float64x2_t x0 = vld1q_f64(samples + i);
                float64x2_t y0 = vmlaq_f64(vmlaq_f64(vmulq_f64(b0, x0), b1, x1), b2, x2);
                y0 = vsubq_f64(y0, vmlaq_f64(vmulq_f64(a1, y1), a2, y2));
                vst1q_f64(samples + i, y0);
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
            m_state.x1 = vgetq_lane_f64(x1, 0);
            m_state.x2 = vgetq_lane_f64(x2, 0);
            m_state.y1 = vgetq_lane_f64(y1, 0);
            m_state.y2 = vgetq_lane_f64(y2, 0);
        }
    }
#endif
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
        normalize_coefficients();
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
        m_coefficients(coefficients) {
        normalize_coefficients();
    }

    auto normalize_coefficients() -> void {
        /// Normalize a0 to 1
        m_coefficients.b0 = m_coefficients.b0 / m_coefficients.a0;
        m_coefficients.b1 = m_coefficients.b1 / m_coefficients.a0;
        m_coefficients.b2 = m_coefficients.b2 / m_coefficients.a0;
        m_coefficients.a1 = m_coefficients.a1 / m_coefficients.a0;
        m_coefficients.a2 = m_coefficients.a2 / m_coefficients.a0;
        m_coefficients.a0 = 1.0;
    }

    /** The filter coefficients */
    Coefficients<T> m_coefficients = {};

    /** The filter state variables */
    State<T> m_state = {};

    /** The number of iterations */
    long long m_iter = 0LL;
};

#endif // DIGITAL_BIQUAD_FILTER_H
