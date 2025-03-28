/// BandPassFilterTest.cpp

#include <gtest/gtest.h>

#include <DigitalBiquad/BandPassFilter.h>

TEST(BandPassFilterTest, CreateValidDoubleFilter) {
    const std::optional<BandPassFilter<double>> filter =
            BandPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(BandPassFilterTest, CreateValidFloatFilter) {
    const std::optional<BandPassFilter<float>> filter =
            BandPassFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(BandPassFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<BandPassFilter<double>> filter =
            BandPassFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(BandPassFilterTest, CreateInvalidFloatFilter) {
    const std::optional<BandPassFilter<float>> filter =
            BandPassFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(BandPassFilterTest, SetCutoffFrequency) {
    std::optional<BandPassFilter<double>> filter =
            BandPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(BandPassFilterTest, SetSampleRate) {
    std::optional<BandPassFilter<double>> filter =
            BandPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(BandPassFilterTest, SetQualityFactor) {
    std::optional<BandPassFilter<double>> filter =
            BandPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(BandPassFilterTest, SetBandwidth) {
    std::optional<BandPassFilter<double>> filter =
            BandPassFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
