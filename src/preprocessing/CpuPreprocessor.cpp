#include "preprocessing/CpuPreprocessor.hpp"
#include <opencv2/imgproc.hpp>

namespace vip {
CpuPreprocessor::CpuPreprocessor(int w, int h) : m_w(w), m_h(h) {}

cv::Mat CpuPreprocessor::process(const cv::Mat& input) {
    if (input.empty()) return cv::Mat();
    cv::Mat out;
    cv::resize(input, out, cv::Size(m_w, m_h));
    if (out.channels() == 3) cv::cvtColor(out, out, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(out, out, cv::Size(3,3), 0);
    return out;
}
} // namespace vip
