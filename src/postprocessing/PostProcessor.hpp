#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

namespace vip {
struct Defect {
    cv::Rect bbox;
    float confidence;
    std::string label;
};

class PostProcessor {
public:
    PostProcessor(float conf = 0.5f);
    std::vector<Defect> detect(const cv::Mat& processed);
    void draw(cv::Mat& frame, const std::vector<Defect>& defects);
private:
    float m_conf;
};
} // namespace vip
