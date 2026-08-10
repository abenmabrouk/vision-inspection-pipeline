#!/bin/bash
set -e

echo "🐳 Build Docker image..."
docker build -t vision-inspection:v1 .

echo "🔨 Compilation dans le conteneur..."
docker run --rm -v "$(pwd):/app" -w /app vision-inspection:v1 bash -c \
    "mkdir -p build && cd build && cmake .. && make -j\$(nproc)"

echo "✅ Build terminé. Exécutable : ./build/inspection_pipeline"
