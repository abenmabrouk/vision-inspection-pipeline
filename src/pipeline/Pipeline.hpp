#pragma once
#include "preprocessing/IProcessor.hpp"
#include "acquisition/FrameAcquisition.hpp"
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <filesystem>

namespace vip {

struct PipelineConfig {
    int fps = 30;
    bool display = true;
    std::string outputDir = "output";  // Dossier de sortie pour les frames
};

struct PipelineMetrics {
    double fps = 0;
    double latencyMs = 0;
    size_t frames = 0;
    size_t defects = 0;
};

class Pipeline {
public:
    Pipeline(std::unique_ptr<IProcessor> preproc, const PipelineConfig& cfg = {});
    ~Pipeline();
    bool start(const std::string& source);
    void stop();
    PipelineMetrics getMetrics() const;
private:
    void processLoop();
    void metricsLoop();
    std::unique_ptr<IProcessor> m_preproc;
    std::unique_ptr<FrameAcquisition> m_acq;
    std::thread m_procThread;
    std::thread m_metThread;
    std::atomic<bool> m_running{false};
    PipelineConfig m_cfg;
    PipelineMetrics m_metrics;
    mutable std::mutex m_mutex;
};

} // namespace vip
