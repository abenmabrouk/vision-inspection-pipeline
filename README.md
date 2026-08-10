Vision Inspection Pipeline
A real-time industrial quality inspection pipeline built with C++17, OpenCV, and Docker. Designed for computer vision applications in manufacturing environments.
🏗️ Architecture
plain
Video Source (File / Webcam / Synthetic)
    │
    ▼
Frame Acquisition (dedicated thread)
    │
    ▼
Preprocessing (CPU: resize, grayscale, blur)
    │
    ▼
Defect Detection (placeholder: variance-based)
    │
    ▼
Post-processing + Annotation
    │
    ▼
Metrics Export (JSON) + Frame Saving (headless mode)
🛠️ Tech Stack
Feuilles de calcul
Technology	Purpose
C++17	High-performance real-time processing
OpenCV 4.x	Image acquisition, preprocessing, visualization
CMake	Cross-platform build system
Docker	Reproducible build and deployment environment
GitHub Actions	CI/CD (optional)
🚀 Quick Start with Docker
Prerequisites
Docker Desktop
Build & Run
bash
# Build the Docker image
docker build -t vision-inspection:v1 .

# Compile the project inside the container
docker run --rm -v $(pwd):/app -w /app vision-inspection:v1 bash -c \
    "mkdir -p build && cd build && cmake .. && make -j$(nproc)"

# Run on the included test video
docker run --rm \
    -v $(pwd):/app \
    -v $(pwd)/data:/app/data \
    -v $(pwd)/output:/app/output \
    -w /app vision-inspection:v1 \
    ./build/inspection_pipeline data/industrial_test_video.mp4 --headless --output output
View Results
After execution, check the output/ directory:
plain
output/
├── metrics.json          # Per-frame latency and defect counts
└── frames/
    ├── frame_0.jpg       # Annotated frames
    ├── frame_1.jpg
    └── ...
🖥️ Native Build (Linux)
bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libopencv-dev

mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./inspection_pipeline demo
📁 Project Structure
plain
.
├── src/
│   ├── acquisition/        # Video capture thread
│   ├── preprocessing/    # CPU image preprocessing
│   ├── postprocessing/   # Defect annotation
│   ├── pipeline/         # Main orchestration loop
│   ├── io/               # Metrics publisher (JSON)
│   └── main.cpp
├── data/
│   └── industrial_test_video.mp4   # Synthetic test video
├── docker/
│   ├── Dockerfile
│   └── docker-compose.yml
├── scripts/
│   ├── docker-build.sh
│   └── docker-run.sh
├── CMakeLists.txt
├── .gitignore
└── README.md
🎬 Test Video
The included data/industrial_test_video.mp4 is a synthetic industrial scene featuring:
Conveyor belt with moving metal parts
Realistic lighting and noise
~45% of parts contain simulated defects (scratches, stains, dents)
6 seconds, 640×480, 30 FPS
📊 Metrics
The pipeline exports per-frame metrics in JSON format:
JSON
[
  {"frame_id": 0, "latency_ms": 8.5, "defect": false},
  {"frame_id": 1, "latency_ms": 7.2, "defect": true}
]
🏷️ Tags
computer-vision cpp opencv docker industrial-vision cmake real-time quality-inspection
📄 License
MIT