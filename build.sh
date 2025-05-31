#!/bin/bash

# Создаем директорию сборки
#mkdir -p build
#cd build

# Конфигурация проекта
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "$@"

# Сборка проекта
cmake --build .

# Статический анализ
cppcheck --enable=all --project=compile_commands.json 2> ./cppcheck.log || true

# Запуск тестов
ctest --output-on-failure > ./test_results.log 2>&1 || true
ctest --output-junit ./TestResults.xml

# Генерация отчета
junit2html ./TestResults.xml ./TestReport.html

# Вывод информации
echo "Сборка завершена успешно!"
echo "Сгенерированные файлы:"
ls -lh ./ | grep -E -i '(\.log|\.xml|\.html)'
