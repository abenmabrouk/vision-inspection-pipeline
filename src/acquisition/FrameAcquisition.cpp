#include "acquisition/FrameAcquisition.hpp"
#include <iostream>
#include <random>

namespace vip {

FrameAcquisition::FrameAcquisition(const std::string& source) : m_source(source) {}
FrameAcquisition::~FrameAcquisition() { stop(); }

bool FrameAcquisition::start() {
    if (m_source == "demo") {
        std::cout << "[INFO] Mode DEMO actif (frames synthétiques)\n";
        m_demoMode = true;
        m_running = true;
        m_thread = std::thread(&FrameAcquisition::demoLoop, this);
        return true;
    }

    if (m_source.size() == 1 && std::isdigit(m_source[0])) {
        m_cap.open(std::stoi(m_source));
    } else {
        m_cap.open(m_source);
    }

    if (!m_cap.isOpened()) {
        std::cerr << "[ERREUR] Impossible d'ouvrir : " << m_source << "\n";
        return false;
    }

    double totalFrames = m_cap.get(cv::CAP_PROP_FRAME_COUNT);
    if (totalFrames > 0) {
        std::cout << "[INFO] Vidéo chargée : " << totalFrames << " frames\n";
    }

    m_running = true;
    m_thread = std::thread(&FrameAcquisition::loop, this);
    return true;
}

void FrameAcquisition::stop() {
    m_running = false;
    if (m_thread.joinable()) m_thread.join();
    if (m_cap.isOpened()) m_cap.release();
}

bool FrameAcquisition::getFrame(cv::Mat& frame) {
    if (!m_hasNewFrame.load()) return false;

    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_frame.empty()) return false;
    frame = m_frame.clone();
    m_hasNewFrame = false;
    return true;
}

void FrameAcquisition::loop() {
    cv::Mat frame;
    while (m_running) {
        if (!m_cap.read(frame)) {
            // Fin de la vidéo fichier -> on s'arrête
            if (!m_demoMode && m_source != "0") {
                std::cout << "[INFO] Fin de la vidéo\n";
                m_finished = true;
                m_running = false;
                break;
            }
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_frame = frame.clone();
            m_hasNewFrame = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }
}

void FrameAcquisition::demoLoop() {
    cv::Mat frame(480, 640, CV_8UC3, cv::Scalar(220, 220, 220));
    int frameCount = 0;
    const int maxDemoFrames = 180; // 6 secondes à 30 FPS

    while (m_running && frameCount < maxDemoFrames) {
        frame = cv::Mat(480, 640, CV_8UC3, cv::Scalar(220, 220, 220));

        int cx = 320 + static_cast<int>(150 * std::sin(frameCount * 0.05));
        int cy = 240 + static_cast<int>(100 * std::cos(frameCount * 0.03));

        cv::rectangle(frame, cv::Point(cx - 80, cy - 60), cv::Point(cx + 80, cy + 60),
                      cv::Scalar(100, 150, 200), -1);
        cv::rectangle(frame, cv::Point(cx - 80, cy - 60), cv::Point(cx + 80, cy + 60),
                      cv::Scalar(50, 50, 50), 2);
        cv::line(frame, cv::Point(cx+10, cy-55), cv::Point(cx+70, cy-55), cv::Scalar(220, 220, 230), 1);

        if (frameCount % 40 > 30) {
            cv::circle(frame, cv::Point(cx + 20, cy - 10), 15, cv::Scalar(40, 40, 40), -1);
        }

        cv::putText(frame, "DEMO MODE - Frame " + std::to_string(frameCount),
                    cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7,
                    cv::Scalar(0, 0, 0), 2);

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_frame = frame.clone();
            m_hasNewFrame = true;
        }

        frameCount++;
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    std::cout << "[INFO] Fin du mode demo\n";
    m_finished = true;
    m_running = false;
}

} // namespace vip
