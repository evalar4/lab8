#!/bin/bash

# Создаем отдельную директорию для сборки с покрытием
mkdir -p build_coverage
cd build_coverage

# Конфигурируем CMake с включенным покрытием
cmake .. -DENABLE_COVERAGE=ON

# Собираем проект
make

# Запускаем тесты
ctest

# Генерируем отчет покрытия
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.filtered.info

# Генерируем HTML отчет (для локального просмотра)
genhtml coverage.filtered.info --output-directory coverage_report

# Выводим информацию о покрытии в консоль
lcov --list coverage.filtered.info


