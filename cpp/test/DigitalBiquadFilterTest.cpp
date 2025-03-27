/// DigitalBiquadFilterTest.cpp

#include <array>
#include <gtest/gtest.h>
#include <optional>
#include <vector>

#include <DigitalBiquad/DigitalBiquadFilter.h>

/**
 * @brief Test the creation of a valid filter
 */
TEST(DigitalBiquadFilterTest, CreateValidDoubleFilter) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    const std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
}

/**
 * @brief Test the creation of a valid filter
 */
TEST(DigitalBiquadFilterTest, CreateValidFloatFilter) {
    constexpr Coefficients<float> coefficients = {.b0 = 1.0f,
                                                  .b1 = 0.0f,
                                                  .b2 = 0.0f,
                                                  .a0 = 1.0f,
                                                  .a1 = 0.0f,
                                                  .a2 = 0.0f};
    const std::optional<DigitalBiquadFilter<float>> filter =
            DigitalBiquadFilter<float>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
}

/**
 * @brief Test the creation of an invalid filter
 */
TEST(DigitalBiquadFilterTest, CreateInvalidDoubleFilter) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 0.0, .a1 = 0.0, .a2 = 0.0};
    const std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_FALSE(filter.has_value());
}

/**
 * @brief Test the creation of an invalid filter
 */
TEST(DigitalBiquadFilterTest, CreateInvalidFloatFilter) {
    constexpr Coefficients<float> coefficients = {.b0 = 1.0f,
                                                  .b1 = 0.0f,
                                                  .b2 = 0.0f,
                                                  .a0 = 0.0f,
                                                  .a1 = 0.0f,
                                                  .a2 = 0.0f};
    const std::optional<DigitalBiquadFilter<float>> filter =
            DigitalBiquadFilter<float>::create(coefficients);
    EXPECT_FALSE(filter.has_value());
}

/**
 * @brief Test the processing of a single sample
 */
TEST(DigitalBiquadFilterTest, ProcessDoubleSample) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    double sample = 1.0;
    filter->process(sample);
    EXPECT_DOUBLE_EQ(sample, 1.0);
}

/**
 * @brief Test the processing of a single sample
 */
TEST(DigitalBiquadFilterTest, ProcessFloatSample) {
    constexpr Coefficients<float> coefficients = {.b0 = 1.0f,
                                                  .b1 = 0.0f,
                                                  .b2 = 0.0f,
                                                  .a0 = 1.0f,
                                                  .a1 = 0.0f,
                                                  .a2 = 0.0f};
    std::optional<DigitalBiquadFilter<float>> filter =
            DigitalBiquadFilter<float>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    float sample = 1.0f;
    filter->process(sample);
    EXPECT_FLOAT_EQ(sample, 1.0f);
}

/**
 * @brief Test the processing of a block of samples
 */
TEST(DigitalBiquadFilterTest, ProcessVectorOfDoubleSamples) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    std::vector<double> samples = {1.0, 0.5, 0.25};
    filter->process(samples.data(), samples.size());
    EXPECT_DOUBLE_EQ(samples[0], 1.0);
    EXPECT_DOUBLE_EQ(samples[1], 0.5);
    EXPECT_DOUBLE_EQ(samples[2], 0.25);
}

/**
 * @brief Test the processing of a block of samples
 */
TEST(DigitalBiquadFilterTest, ProcessVectorOfFloatSamples) {
    constexpr Coefficients<float> coefficients = {.b0 = 1.0f,
                                                  .b1 = 0.0f,
                                                  .b2 = 0.0f,
                                                  .a0 = 1.0f,
                                                  .a1 = 0.0f,
                                                  .a2 = 0.0f};
    std::optional<DigitalBiquadFilter<float>> filter =
            DigitalBiquadFilter<float>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    std::vector<float> samples = {1.0f, 0.5f, 0.25f};
    filter->process(samples.data(), samples.size());
    EXPECT_FLOAT_EQ(samples[0], 1.0f);
    EXPECT_FLOAT_EQ(samples[1], 0.5f);
    EXPECT_FLOAT_EQ(samples[2], 0.25f);
}

/**
 * @brief Test the processing of a block of samples
 */
TEST(DigitalBiquadFilterTest, ProcessArrayOfDoubleSamples) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    std::array<double, 3> samples = {1.0, 0.5, 0.25};
    filter->process(samples.data(), samples.size());
    EXPECT_DOUBLE_EQ(samples[0], 1.0);
    EXPECT_DOUBLE_EQ(samples[1], 0.5);
    EXPECT_DOUBLE_EQ(samples[2], 0.25);
}

/**
 * @brief Test the processing of a block of samples
 */
TEST(DigitalBiquadFilterTest, ProcessArrayOfFloatSamples) {
    constexpr Coefficients<float> coefficients = {.b0 = 1.0f,
                                                  .b1 = 0.0f,
                                                  .b2 = 0.0f,
                                                  .a0 = 1.0f,
                                                  .a1 = 0.0f,
                                                  .a2 = 0.0f};
    std::optional<DigitalBiquadFilter<float>> filter =
            DigitalBiquadFilter<float>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    std::array<float, 3> samples = {1.0f, 0.5f, 0.25f};
    filter->process(samples.data(), samples.size());
    EXPECT_FLOAT_EQ(samples[0], 1.0f);
    EXPECT_FLOAT_EQ(samples[1], 0.5f);
    EXPECT_FLOAT_EQ(samples[2], 0.25f);
}

/**
 * @brief Test the reset of the filter
 */
TEST(DigitalBiquadFilterTest, ResetFilter) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    double sample = 1.0;
    filter->process(sample);
    filter->reset();
    double newSample = 1.0;
    filter->process(newSample);
    EXPECT_DOUBLE_EQ(newSample, 1.0);
}

/**
 * @brief Test processing with zero coefficients
 */
TEST(DigitalBiquadFilterTest, ProcessWithZeroCoefficients) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 0.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    double sample = 1.0;
    filter->process(sample);
    EXPECT_DOUBLE_EQ(sample, 0.0);
}

/**
 * @brief Test processing a block of zeros
 */
TEST(DigitalBiquadFilterTest, ProcessBlockOfZeros) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    std::array<double, 5> samples = {0.0, 0.0, 0.0, 0.0, 0.0};
    filter->process(samples.data(), samples.size());
    EXPECT_DOUBLE_EQ(samples[0], 0.0);
}

/**
 * @brief Test processing a ramp signal
 */
TEST(DigitalBiquadFilterTest, ProcessRampSignal) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    std::optional<DigitalBiquadFilter<double>> filter =
            DigitalBiquadFilter<double>::create(coefficients);
    EXPECT_TRUE(filter.has_value());
    std::array<double, 5> samples = {0.0, 1.0, 2.0, 3.0, 4.0};
    filter->process(samples.data(), samples.size());
    EXPECT_DOUBLE_EQ(samples[0], 0.0);
    EXPECT_DOUBLE_EQ(samples[1], 1.0);
    EXPECT_DOUBLE_EQ(samples[2], 2.0);
    EXPECT_DOUBLE_EQ(samples[3], 3.0);
    EXPECT_DOUBLE_EQ(samples[4], 4.0);
}

/**
 * @brief Test for stability with large input values
 */
TEST(DigitalBiquadFilterTest, ProcessLargeValues) {
    constexpr Coefficients<double> coefficients = {
            .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    auto filter = DigitalBiquadFilter<double>::create(coefficients).value();
    double largeSample = 1e6;
    filter.process(largeSample);
    EXPECT_DOUBLE_EQ(largeSample, largeSample);
}

/**
 * @brief Test processing with negative coefficients
 */
TEST(DigitalBiquadFilterTest, ProcessWithNegativeCoefficients) {
    constexpr Coefficients<double> coefficients = {
            .b0 = -1.0, .b1 = 0.0, .b2 = 0.0, .a0 = 1.0, .a1 = 0.0, .a2 = 0.0};
    auto filter = DigitalBiquadFilter<double>::create(coefficients).value();
    double sample = 1.0;
    filter.process(sample);
    EXPECT_DOUBLE_EQ(sample, -1.0);
}
