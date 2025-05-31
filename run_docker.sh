#!/bin/bash
# run_docker.sh

# Собираем Docker-образ
docker build -t project-builder .

# Запускаем контейнер:
# - Монтируем текущую директорию в /app контейнера
# - Запускаем скрипт сборки
docker run --rm -v "$(pwd):/app" project-builder /bin/bash -c "cd /app && ./build.sh"
