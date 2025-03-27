/// LowShelfFilterTest.cpp

#include <gtest/gtest.h>

#include <DigitalBiquad/LowShelfFilter.h>

TEST(LowShelfFilterTest, CreateValidDoubleFilter) {
    const std::optional<LowShelfFilter<double>> filter =
            LowShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(LowShelfFilterTest, CreateValidFloatFilter) {
    const std::optional<LowShelfFilter<float>> filter =
            LowShelfFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(LowShelfFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<LowShelfFilter<double>> filter =
            LowShelfFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(LowShelfFilterTest, CreateInvalidFloatFilter) {
    const std::optional<LowShelfFilter<float>> filter =
            LowShelfFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(LowShelfFilterTest, SetCutoffFrequency) {
    std::optional<LowShelfFilter<double>> filter =
            LowShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(LowShelfFilterTest, SetSampleRate) {
    std::optional<LowShelfFilter<double>> filter =
            LowShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(LowShelfFilterTest, SetQualityFactor) {
    std::optional<LowShelfFilter<double>> filter =
            LowShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(LowShelfFilterTest, SetBandwidth) {
    std::optional<LowShelfFilter<double>> filter =
            LowShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
