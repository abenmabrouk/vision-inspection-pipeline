#include "pipeline/Pipeline.hpp"
#include "postprocessing/PostProcessor.hpp"
#include "io/MetricsPublisher.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <chrono>

namespace vip {

Pipeline::Pipeline(std::unique_ptr<IProcessor> preproc, const PipelineConfig& cfg)
    : m_preproc(std::move(preproc)), m_cfg(cfg) {}

Pipeline::~Pipeline() { stop(); }

bool Pipeline::start(const std::string& source) {
    if (!m_cfg.outputDir.empty()) {
        std::filesystem::create_directories(m_cfg.outputDir + "/frames");
    }

    m_acq = std::make_unique<FrameAcquisition>(source);
    if (!m_acq->start()) return false;
    m_running = true;
    m_procThread = std::thread(&Pipeline::processLoop, this);
    m_metThread = std::thread(&Pipeline::metricsLoop, this);
    return true;
}

void Pipeline::stop() {
    m_running = false;
    if (m_procThread.joinable()) m_procThread.join();
    if (m_metThread.joinable()) m_metThread.join();
    if (m_acq) m_acq->stop();
}

void Pipeline::processLoop() {
    PostProcessor post;
    MetricsPublisher pub;
    cv::Mat frame, proc;
    size_t id = 0;

    while (m_running) {
        auto t0 = std::chrono::high_resolution_clock::now();

        // Attendre une nouvelle frame (bloque si pas de nouvelle frame)
        if (!m_acq->getFrame(frame)) {
            // Si acquisition terminée et plus de nouvelle frame -> sortir
            if (m_acq->isFinished()) {
                std::cout << "\n[INFO] Traitement terminé. " << id << " frames analysées.\n";
                m_running = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        proc = m_preproc->process(frame);
        auto defects = post.detect(proc);

        if (m_cfg.display) {
            cv::Mat disp = frame.clone();
            post.draw(disp, defects);
            double lat = std::chrono::duration<double,std::milli>(
                std::chrono::high_resolution_clock::now() - t0).count();
            cv::putText(disp, "Lat: " + std::to_string(int(lat)) + " ms", {10,30},
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, {0,255,0}, 2);
            cv::imshow("Inspection", disp);
            if (cv::waitKey(1) == 27) m_running = false;
        }

        if (!m_cfg.outputDir.empty()) {
            cv::Mat disp = frame.clone();
            post.draw(disp, defects);
            double lat = std::chrono::duration<double,std::milli>(
                std::chrono::high_resolution_clock::now() - t0).count();
            cv::putText(disp, "Lat: " + std::to_string(int(lat)) + " ms", {10,30},
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, {0,255,0}, 2);
            std::string filename = m_cfg.outputDir + "/frames/frame_" + 
                                   std::to_string(id) + ".jpg";
            cv::imwrite(filename, disp);
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_metrics.frames++;
            m_metrics.defects += defects.size();
            m_metrics.latencyMs = std::chrono::duration<double,std::milli>(
                std::chrono::high_resolution_clock::now() - t0).count();
        }
        id++;
    }

    if (m_cfg.display) cv::destroyAllWindows();
    pub.saveJson(m_cfg.outputDir + "/metrics.json");
}

void Pipeline::metricsLoop() {
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(m_mutex);
        static size_t last = 0;
        m_metrics.fps = double(m_metrics.frames - last);
        last = m_metrics.frames;
    }
}

PipelineMetrics Pipeline::getMetrics() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_metrics;
}

} // namespace vip
