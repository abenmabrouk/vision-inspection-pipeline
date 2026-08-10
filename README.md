# Vision Inspection Pipeline

**Real-time industrial quality inspection pipeline built with C++17, OpenCV, CMake, and Docker.**

This project demonstrates a modular computer vision pipeline designed for **real-time quality inspection in manufacturing environments**.

The pipeline acquires video frames, performs CPU-based preprocessing, detects potential defects, annotates the frames, and exports inspection metrics in JSON format.

---

## 🚀 Features

* Real-time video frame acquisition
* Dedicated acquisition thread
* CPU-based image preprocessing
* Defect detection pipeline
* Frame annotation and post-processing
* Per-frame latency measurement
* JSON metrics export
* Annotated frame export
* Headless execution mode
* Dockerized build and execution
* Modular C++17 architecture
* CMake-based build system

---

## 🏗️ Architecture

```text
                  Video Source
             (File / Webcam / Synthetic)
                        │
                        ▼
              ┌───────────────────┐
              │ Frame Acquisition │
              │  Dedicated Thread │
              └─────────┬─────────┘
                        │
                        ▼
              ┌───────────────────┐
              │   Preprocessing   │
              │ Resize / Gray /   │
              │      Blur         │
              └─────────┬─────────┘
                        │
                        ▼
              ┌───────────────────┐
              │  Defect Detection │
              │ Variance-based    │
              │   Placeholder     │
              └─────────┬─────────┘
                        │
                        ▼
              ┌───────────────────┐
              │  Post-processing  │
              │    Annotation     │
              └─────────┬─────────┘
                        │
                        ▼
          ┌─────────────────────────────┐
          │       Output / Metrics      │
          │                             │
          │ • JSON inspection metrics   │
          │ • Annotated frames          │
          └─────────────────────────────┘
```

---

## 🛠️ Tech Stack

| Technology         | Purpose                                       |
| ------------------ | --------------------------------------------- |
| **C++17**          | High-performance real-time processing         |
| **OpenCV 4.x**     | Image acquisition and computer vision         |
| **CMake**          | Cross-platform build system                   |
| **Docker**         | Reproducible build and deployment environment |
| **GitHub Actions** | Continuous integration (optional)             |

---

# 🚀 Quick Start

## Prerequisites

* Docker Desktop
* Git

No local C++ or OpenCV installation is required when using Docker.

---

## 🐳 Build with Docker

Clone the repository:

```bash
git clone <repository-url>
cd vision-inspection
```

Build the Docker image:

```bash
docker build -t vision-inspection:v1 .
```

Compile the project inside the container:

```bash
docker run --rm \
  -v "$(pwd):/app" \
  -w /app \
  vision-inspection:v1 \
  bash -c "mkdir -p build && cd build && cmake .. && make -j$(nproc)"
```

---

## ▶️ Run the Pipeline

Run the included test video in headless mode:

```bash
docker run --rm \
  -v "$(pwd):/app" \
  -v "$(pwd)/data:/app/data" \
  -v "$(pwd)/output:/app/output" \
  -w /app \
  vision-inspection:v1 \
  ./build/inspection_pipeline \
  data/industrial_test_video.mp4 \
  --headless \
  --output output
```

---

## 📊 View Results

After execution, the generated files are available in the `output/` directory:

```text
output/
├── metrics.json
└── frames/
    ├── frame_0.jpg
    ├── frame_1.jpg
    ├── frame_2.jpg
    └── ...
```

### `metrics.json`

Contains per-frame processing metrics and defect detection results.

Example:

```json
[
  {
    "frame_id": 0,
    "latency_ms": 8.5,
    "defect": false
  },
  {
    "frame_id": 1,
    "latency_ms": 7.2,
    "defect": true
  }
]
```

---

# 🖥️ Native Build — Linux

If you prefer to build the project directly on Linux:

### Install dependencies

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev
```

### Build

```bash
mkdir build
cd build

cmake ..
make -j$(nproc)
```

### Run

```bash
./inspection_pipeline ../data/industrial_test_video.mp4 \
    --headless \
    --output ../output
```

---

# 📁 Project Structure

```text
.
├── src/
│   ├── acquisition/
│   │   └── # Video capture and acquisition thread
│   │
│   ├── preprocessing/
│   │   └── # CPU image preprocessing
│   │
│   ├── postprocessing/
│   │   └── # Defect annotation and post-processing
│   │
│   ├── pipeline/
│   │   └── # Main pipeline orchestration
│   │
│   ├── io/
│   │   └── # JSON metrics export
│   │
│   └── main.cpp
│
├── data/
│   └── industrial_test_video.mp4
│
├── docker/
│   ├── Dockerfile
│   └── docker-compose.yml
│
├── scripts/
│   ├── docker-build.sh
│   └── docker-run.sh
│
├── CMakeLists.txt
├── .gitignore
└── README.md
```

---

# 🎬 Test Video

The repository includes a synthetic industrial inspection video:

`data/industrial_test_video.mp4`

The test sequence simulates a manufacturing environment with:

* Conveyor belt
* Moving metal parts
* Industrial-style lighting
* Image noise
* Simulated surface defects
* Scratches
* Stains
* Dents

### Video specifications

| Property     | Value      |
| ------------ | ---------- |
| Duration     | ~6 seconds |
| Resolution   | 640 × 480  |
| Frame rate   | 30 FPS     |
| Defect ratio | ~45%       |
| Source       | Synthetic  |

The video is intended for reproducible testing of the inspection pipeline.

---

# 📈 Performance Metrics

The pipeline records per-frame processing metrics, including:

* Frame ID
* Processing latency
* Defect detection result

Example:

```json
[
  {
    "frame_id": 0,
    "latency_ms": 8.5,
    "defect": false
  },
  {
    "frame_id": 1,
    "latency_ms": 7.2,
    "defect": true
  }
]
```

This makes it possible to evaluate the pipeline's **processing latency and real-time behavior**.

---

# 🔬 Defect Detection

The current implementation uses a **variance-based defect detection method** as a lightweight placeholder.

The architecture is intentionally modular so that the detection component can later be replaced by more advanced approaches, such as:

* Classical image processing
* Feature-based detection
* Machine learning classifiers
* Deep learning models
* ONNX Runtime inference
* CUDA-accelerated processing

---

# 🧩 Docker

Docker provides a reproducible environment containing the required build tools and OpenCV dependencies.

This allows the project to be built and executed without installing the complete C++/OpenCV toolchain directly on the host machine.

---

# 🧪 Development

The project is structured around independent pipeline stages:

```text
Acquisition
     ↓
Preprocessing
     ↓
Detection
     ↓
Post-processing
     ↓
Metrics / Output
```

Each stage can be developed and optimized independently.

---

# 🏷️ Tags

`computer-vision` · `cpp` · `cpp17` · `opencv` · `docker` · `cmake` · `real-time` · `industrial-vision` · `quality-inspection` · `manufacturing` · `image-processing`

---

# 📄 License

This project is licensed under the **MIT License**.
