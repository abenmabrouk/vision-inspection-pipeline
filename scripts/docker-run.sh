#!/bin/bash

SOURCE="\${1:-demo}"
OUTPUT="\${2:-output}"

echo "🚀 Lancement du pipeline..."
echo "   Source : \$SOURCE"
echo "   Output : \$OUTPUT/"

# Créer les dossiers si besoin
mkdir -p \$OUTPUT/frames
mkdir -p data

docker run --rm \
  -v "$(pwd):/app" \
  -v "$(pwd)/data:/app/data" \
  -v "$(pwd)/\$OUTPUT:/app/\$OUTPUT" \
  -w /app \
  vision-inspection:v1 \
  ./build/inspection_pipeline "\$SOURCE" --headless --output "\$OUTPUT"
