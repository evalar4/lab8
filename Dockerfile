FROM ubuntu:22.04

# Установка зависимостей
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

# Сборка Google Test
RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

# Установка junit2html
RUN pip3 install junit2html

# Копирование скрипта сборки
COPY build.sh /build.sh
RUN chmod +x /build.sh

# Рабочая директория
WORKDIR /workspace

# Точка входа
ENTRYPOINT ["/build.sh"]
