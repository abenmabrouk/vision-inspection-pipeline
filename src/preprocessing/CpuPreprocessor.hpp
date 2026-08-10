#pragma once
#include "preprocessing/IProcessor.hpp"

namespace vip {
class CpuPreprocessor : public IProcessor {
public:
    CpuPreprocessor(int w = 640, int h = 480);
    cv::Mat process(const cv::Mat& input) override;
private:
    int m_w, m_h;
};
} // namespace vip
