/// HighShelfFilterTest.cpp

#include <gtest/gtest.h>

#include "HighShelfFilter.h"

TEST(HighShelfFilterTest, CreateValidDoubleFilter) {
    const std::optional<HighShelfFilter<double>> filter =
            HighShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(HighShelfFilterTest, CreateValidFloatFilter) {
    const std::optional<HighShelfFilter<float>> filter =
            HighShelfFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(HighShelfFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<HighShelfFilter<double>> filter =
            HighShelfFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(HighShelfFilterTest, CreateInvalidFloatFilter) {
    const std::optional<HighShelfFilter<float>> filter =
            HighShelfFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(HighShelfFilterTest, SetCutoffFrequency) {
    std::optional<HighShelfFilter<double>> filter =
            HighShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(HighShelfFilterTest, SetSampleRate) {
    std::optional<HighShelfFilter<double>> filter =
            HighShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(HighShelfFilterTest, SetQualityFactor) {
    std::optional<HighShelfFilter<double>> filter =
            HighShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(HighShelfFilterTest, SetBandwidth) {
    std::optional<HighShelfFilter<double>> filter =
            HighShelfFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
