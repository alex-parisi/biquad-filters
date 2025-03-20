/// HighPassFilterTest.cpp

#include <gtest/gtest.h>

#include "HighPassFilter.h"

TEST(HighPassFilterTest, CreateValidDoubleFilter) {
    const std::optional<HighPassFilter<double>> filter =
            HighPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(HighPassFilterTest, CreateValidFloatFilter) {
    const std::optional<HighPassFilter<float>> filter =
            HighPassFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(HighPassFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<HighPassFilter<double>> filter =
            HighPassFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(HighPassFilterTest, CreateInvalidFloatFilter) {
    const std::optional<HighPassFilter<float>> filter =
            HighPassFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(HighPassFilterTest, SetCutoffFrequency) {
    std::optional<HighPassFilter<double>> filter =
            HighPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(HighPassFilterTest, SetSampleRate) {
    std::optional<HighPassFilter<double>> filter =
            HighPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(HighPassFilterTest, SetQualityFactor) {
    std::optional<HighPassFilter<double>> filter =
            HighPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(HighPassFilterTest, SetBandwidth) {
    std::optional<HighPassFilter<double>> filter =
            HighPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
