/// AllPassFilterTest.cpp

#include <gtest/gtest.h>

#include "AllPassFilter.h"

TEST(AllPassFilterTest, CreateValidDoubleFilter) {
    const std::optional<AllPassFilter<double>> filter =
            AllPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(AllPassFilterTest, CreateValidFloatFilter) {
    const std::optional<AllPassFilter<float>> filter =
            AllPassFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(AllPassFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<AllPassFilter<double>> filter =
            AllPassFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(AllPassFilterTest, CreateInvalidFloatFilter) {
    const std::optional<AllPassFilter<float>> filter =
            AllPassFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(AllPassFilterTest, SetCutoffFrequency) {
    std::optional<AllPassFilter<double>> filter =
            AllPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(AllPassFilterTest, SetSampleRate) {
    std::optional<AllPassFilter<double>> filter =
            AllPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(AllPassFilterTest, SetQualityFactor) {
    std::optional<AllPassFilter<double>> filter =
            AllPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(AllPassFilterTest, SetBandwidth) {
    std::optional<AllPassFilter<double>> filter =
            AllPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
