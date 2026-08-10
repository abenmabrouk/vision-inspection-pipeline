#include <iostream>
#include <csignal>
#include <memory>
#include "preprocessing/CpuPreprocessor.hpp"
#include "pipeline/Pipeline.hpp"

std::atomic<bool> g_run{true};
void onSignal(int) { g_run = false; std::cout << "\n[Signal] Arrêt...\n"; }

void printUsage(const char* prog) {
    std::cout << "Usage: " << prog << " <source> [options]\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  source          : demo, 0 (webcam), ou chemin video\n\n";
    std::cout << "Options:\n";
    std::cout << "  --headless      : pas d'affichage GUI (mode Docker)\n";
    std::cout << "  --output <dir>  : dossier de sortie frames + metrics (defaut: output)\n";
    std::cout << "\nExemples:\n";
    std::cout << "  " << prog << " demo --headless --output output\n";
    std::cout << "  " << prog << " video.mp4 --output results/\n";
}

int main(int argc, char** argv) {
    std::signal(SIGINT, onSignal);

    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string src = argv[1];
    bool headless = false;
    std::string outputDir = "output";

    for (int i = 2; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--headless") headless = true;
        else if (arg == "--output" && i + 1 < argc) outputDir = argv[++i];
    }

    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║  Vision Inspection Pipeline v1.0     ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "Source   : " << src << "\n";
    std::cout << "Display  : " << (headless ? "OFF (headless)" : "ON") << "\n";
    std::cout << "Output   : " << outputDir << "/\n\n";

    auto preproc = std::make_unique<vip::CpuPreprocessor>(640, 480);
    vip::PipelineConfig cfg;
    cfg.display = !headless;
    cfg.outputDir = outputDir;

    vip::Pipeline pipe(std::move(preproc), cfg);
    if (!pipe.start(src)) {
        std::cerr << "[ERREUR] Échec démarrage\n";
        return 1;
    }

    while (g_run) {
        auto m = pipe.getMetrics();
        std::cout << "\r[FPS:" << int(m.fps) << " Lat:" << int(m.latencyMs) 
                  << "ms Frames:" << m.frames << " Defauts:" << m.defects << "]" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    pipe.stop();
    std::cout << "\n[INFO] Arrêté.\n";
    std::cout << "[INFO] Résultats dans : " << outputDir << "/\n";
    std::cout << "[INFO]   - metrics.json\n";
    std::cout << "[INFO]   - frames/*.jpg\n";
    return 0;
}
