#!/bin/bash

# Создаем директорию сборки
mkdir -p build
cd build

# Конфигурация проекта
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "$@" || exit 1

# Сборка проекта
cmake --build . || exit 1

# Статический анализ кода
cppcheck --enable=all --project=compile_commands.json 2> ../cppcheck.log || true

# Запуск тестов
ctest --output-on-failure -T Test > ../test_results.log 2>&1 || true
ctest --output-junit TestResults.xml

# Генерация HTML-отчета
junit2html TestResults.xml TestReport.html

# Вывод информации
echo "Build successful!"
echo "Generated files:"
ls -lh ../*.log ../*.xml ../*.html
