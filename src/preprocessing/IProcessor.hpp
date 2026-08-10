#pragma once
#include <opencv2/opencv.hpp>

namespace vip {
class IProcessor {
public:
    virtual ~IProcessor() = default;
    virtual cv::Mat process(const cv::Mat& input) = 0;
};
} // namespace vip
