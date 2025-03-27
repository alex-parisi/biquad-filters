/// NotchFilterTest.cpp

#include <gtest/gtest.h>

#include <DigitalBiquad/NotchFilter.h>

TEST(NotchFilterTest, CreateValidDoubleFilter) {
    const std::optional<NotchFilter<double>> filter =
            NotchFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(NotchFilterTest, CreateValidFloatFilter) {
    const std::optional<NotchFilter<float>> filter =
            NotchFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(NotchFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<NotchFilter<double>> filter =
            NotchFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(NotchFilterTest, CreateInvalidFloatFilter) {
    const std::optional<NotchFilter<float>> filter =
            NotchFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(NotchFilterTest, SetCutoffFrequency) {
    std::optional<NotchFilter<double>> filter =
            NotchFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(NotchFilterTest, SetSampleRate) {
    std::optional<NotchFilter<double>> filter =
            NotchFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(NotchFilterTest, SetQualityFactor) {
    std::optional<NotchFilter<double>> filter =
            NotchFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(NotchFilterTest, SetBandwidth) {
    std::optional<NotchFilter<double>> filter =
            NotchFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
