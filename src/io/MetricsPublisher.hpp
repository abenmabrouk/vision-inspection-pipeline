#pragma once
#include <string>
#include <vector>
#include <mutex>

namespace vip {
struct FrameMetrics {
    double latencyMs = 0;
    bool defect = false;
    size_t frameId = 0;
};

class MetricsPublisher {
public:
    void publish(const FrameMetrics& m);
    void print() const;
    void saveJson(const std::string& path) const;
private:
    mutable std::mutex m_mutex;
    std::vector<FrameMetrics> m_history;
};
} // namespace vip
