#!/bin/bash

set -e  # Прерывание при ошибках

# Установка зависимостей
apt-get update
apt-get install -y lcov

# Сборка с покрытием
mkdir -p build_coverage
cd build_coverage
cmake .. -DENABLE_COVERAGE=ON
make
ctest

# Генерация отчета
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info \
    '/usr/*' \
    '*/tests/*' \
    '*/googletest/*' \
    '*/formatter_ex_lib/*' \
    '*/solver_lib/*' \
    --output-file coverage.filtered.info

# Вывод информации для отладки
echo "=== Coverage Information ==="
lcov --list coverage.filtered.info

# Генерация HTML отчета
genhtml coverage.filtered.info --output-directory coverage_report
