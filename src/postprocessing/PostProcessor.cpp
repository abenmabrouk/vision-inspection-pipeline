#include "postprocessing/PostProcessor.hpp"

namespace vip {
PostProcessor::PostProcessor(float conf) : m_conf(conf) {}

std::vector<Defect> PostProcessor::detect(const cv::Mat& processed) {
    std::vector<Defect> defects;
    if (processed.empty()) return defects;

    // Détection simple par variance (placeholder pour v1)
    cv::Mat mean, stddev;
    cv::meanStdDev(processed, mean, stddev);
    if (stddev.at<double>(0) > 45.0) {
        Defect d;
        d.bbox = cv::Rect(100, 100, 200, 150);
        d.confidence = 0.72f;
        d.label = "anomalie";
        defects.push_back(d);
    }
    return defects;
}

void PostProcessor::draw(cv::Mat& frame, const std::vector<Defect>& defects) {
    for (const auto& d : defects) {
        cv::rectangle(frame, d.bbox, cv::Scalar(0,0,255), 2);
        std::string txt = d.label + " " + std::to_string(int(d.confidence*100)) + "%";
        cv::putText(frame, txt, cv::Point(d.bbox.x, d.bbox.y-5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,255), 1);
    }
}
} // namespace vip
