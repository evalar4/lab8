#!/bin/bash
# build.sh
set -e

# Создаем директорию для сборки
mkdir -p build
cd build

# Конфигурируем и собираем проект
cmake .. && make

# Запускаем тесты и сохраняем журнал
ctest --verbose > ../test_results.log 2>&1 || (echo "Tests failed! Check test_results.log"; exit 1)

echo "Build successful! Output: build/, test_results.log"
