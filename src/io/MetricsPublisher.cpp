#include "io/MetricsPublisher.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

namespace vip {
void MetricsPublisher::publish(const FrameMetrics& m) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_history.push_back(m);
}

void MetricsPublisher::print() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_history.empty()) return;
    double avg = 0;
    size_t defects = 0;
    for (const auto& m : m_history) {
        avg += m.latencyMs;
        if (m.defect) defects++;
    }
    avg /= m_history.size();
    std::cout << "\n=== Métriques ===\n";
    std::cout << "Frames: " << m_history.size() << " | Latence moy: " << std::fixed << std::setprecision(2) << avg << " ms | Défauts: " << defects << "\n";
}

void MetricsPublisher::saveJson(const std::string& path) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream f(path);
    f << "[\n";
    for (size_t i = 0; i < m_history.size(); ++i) {
        f << "  {\"frame_id\":" << m_history[i].frameId << ",\"latency_ms\":" << m_history[i].latencyMs << ",\"defect\":" << (m_history[i].defect?"true":"false") << "}";
        if (i < m_history.size()-1) f << ",";
        f << "\n";
    }
    f << "]\n";
}
} // namespace vip
