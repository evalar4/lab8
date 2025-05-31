# Dockerfile
FROM ubuntu:latest

# Установка зависимостей
RUN apt-get update && \
    apt-get install -y build-essential cmake lcov g++ && \
    rm -rf /var/lib/apt/lists/*

# Создаем рабочую директорию
WORKDIR /app
