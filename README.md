# Vision Inspection Pipeline v1.0

Pipeline C++17 d'inspection qualité par vision industrielle.  
**Packagé avec Docker** pour un build reproductible et portable.

## 🐳 Quick Start avec Docker (recommandé)

### 1. Build image + compilation
```bash
./scripts/docker-build.sh
```

### 2. Lancer le pipeline
```bash
# Mode démo (webcam simulée, headless)
./scripts/docker-run.sh demo

# Avec un fichier vidéo (monté en volume)
./scripts/docker-run.sh data/video.mp4
```

### 3. Ou avec docker-compose
```bash
docker-compose up vision-build   # compile
docker-compose up vision-run     # exécute
```

## 🖥️ Build natif (Linux)
```bash
sudo apt-get install build-essential cmake libopencv-dev
mkdir build && cd build
cmake ..
make -j$(nproc)
./inspection_pipeline demo
```

## 📁 Architecture v1
- `src/acquisition/` : Capture vidéo multithread
- `src/preprocessing/` : Resize, grayscale, blur (CPU OpenCV)
- `src/postprocessing/` : Détection placeholder + dessin
- `src/pipeline/` : Orchestration + métriques
- `src/io/` : Export JSON des métriques

## 🏷️ Tags
`docker` `opencv` `c++17` `computer-vision` `cmake` `devops`
