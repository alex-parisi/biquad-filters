/// PeakingEQFilterTest.cpp

#include <gtest/gtest.h>

#include <DigitalBiquad/PeakingEQFilter.h>

TEST(PeakingEQFilterTest, CreateValidDoubleFilter) {
    const std::optional<PeakingEQFilter<double>> filter =
            PeakingEQFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(PeakingEQFilterTest, CreateValidFloatFilter) {
    const std::optional<PeakingEQFilter<float>> filter =
            PeakingEQFilter<float>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
}

TEST(PeakingEQFilterTest, CreateInvalidDoubleFilter) {
    const std::optional<PeakingEQFilter<double>> filter =
            PeakingEQFilter<double>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(PeakingEQFilterTest, CreateInvalidFloatFilter) {
    const std::optional<PeakingEQFilter<float>> filter =
            PeakingEQFilter<float>::create(1000.0, 0);
    EXPECT_FALSE(filter.has_value());
}

TEST(PeakingEQFilterTest, SetCutoffFrequency) {
    std::optional<PeakingEQFilter<double>> filter =
            PeakingEQFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 1000.0);
    filter->set_cutoff(2000.0);
    EXPECT_DOUBLE_EQ(filter->get_cutoff(), 2000.0);
}

TEST(PeakingEQFilterTest, SetSampleRate) {
    std::optional<PeakingEQFilter<double>> filter =
            PeakingEQFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_EQ(filter->get_sample_rate(), 44100);
    filter->set_sample_rate(48000);
    EXPECT_EQ(filter->get_sample_rate(), 48000);
}

TEST(PeakingEQFilterTest, SetQualityFactor) {
    std::optional<PeakingEQFilter<double>> filter =
            PeakingEQFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 0.7071067811865476);
    filter->set_q_factor(1.0);
    EXPECT_DOUBLE_EQ(filter->get_q_factor(), 1.0);
}

TEST(PeakingEQFilterTest, SetBandwidth) {
    std::optional<PeakingEQFilter<double>> filter =
            PeakingEQFilter<double>::create(1000.0, 44100);
    EXPECT_TRUE(filter.has_value());
    filter->set_bandwidth(2.0);
    EXPECT_DOUBLE_EQ(filter->get_bandwidth(), 2.0);
}
