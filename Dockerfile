FROM ubuntu:22.04

# Установка всех зависимостей
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    cppcheck \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Сборка и установка Google Test (исправленная версия)
RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

# Установка junit2html для отчетов
RUN pip3 install junit2html

# Создаем рабочую директорию
WORKDIR /app

# Команда сборки проекта
CMD mkdir -p build && \
    cd build && \
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && \
    cmake --build . && \
    cppcheck --enable=all --project=compile_commands.json 2> ../cppcheck.log || true && \
    ctest --output-on-failure -T Test > ../test_results.log 2>&1 || true && \
    ctest --output-junit TestResults.xml && \
    junit2html TestResults.xml TestReport.html && \
    echo "Build successful!" && \
    echo "Generated files:" && \
    echo " - test_results.log" && \
    echo " - cppcheck.log" && \
    echo " - TestResults.xml" && \
    echo " - TestReport.html"
