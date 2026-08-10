#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

namespace vip {

class FrameAcquisition {
public:
    explicit FrameAcquisition(const std::string& source);
    ~FrameAcquisition();
    bool start();
    void stop();
    bool getFrame(cv::Mat& frame);
    bool isRunning() const { return m_running; }
    bool isFinished() const { return m_finished; }

private:
    void loop();
    void demoLoop();

    std::string m_source;
    cv::VideoCapture m_cap;
    std::thread m_thread;
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_demoMode{false};
    std::atomic<bool> m_finished{false};
    std::atomic<bool> m_hasNewFrame{false};
    cv::Mat m_frame;
    std::mutex m_mutex;
};

} // namespace vip
