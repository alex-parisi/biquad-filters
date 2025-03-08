/// LowPassFilterTest.cpp

#include <gtest/gtest.h>

#include "LowPassFilter.h"

TEST(LowPassFilterTest, CreateValidDoubleFilter) {
    const std::optional<LowPassFilter<double>> filter =
            LowPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(LowPassFilterTest, CreateValidFloatFilter) {
    const std::optional<LowPassFilter<float>> filter =
            LowPassFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(LowPassFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<LowPassFilter<double>> filter =
            LowPassFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(LowPassFilterTest, CreateInvalidFloatFilter) {
    const std::optional<LowPassFilter<float>> filter =
            LowPassFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(LowPassFilterTest, SetCutoffFrequency) {
    std::optional<LowPassFilter<double>> filter =
            LowPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(LowPassFilterTest, SetSampleRate) {
    std::optional<LowPassFilter<double>> filter =
            LowPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(LowPassFilterTest, SetQualityFactor) {
    std::optional<LowPassFilter<double>> filter =
            LowPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(LowPassFilterTest, SetBandwidth) {
    std::optional<LowPassFilter<double>> filter =
            LowPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
