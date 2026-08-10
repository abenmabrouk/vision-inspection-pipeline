#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

TEST(Basic, FrameNotEmpty) {
    cv::Mat m(100, 100, CV_8UC3, cv::Scalar(50,100,150));
    EXPECT_FALSE(m.empty());
    EXPECT_EQ(m.rows, 100);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
