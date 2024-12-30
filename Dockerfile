# Используем официальный образ Ubuntu в качестве основы
FROM ubuntu:20.04

# Устанавливаем переменные окружения для автоматической настройки
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

# Устанавливаем необходимые зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    libboost-system-dev \
    libcurl4-openssl-dev \
    zlib1g-dev \
    libsqlite3-dev \
    tzdata \
    wget \
    git \
    && apt-get clean

# Устанавливаем TgBot библиотеку
RUN git clone https://github.com/reo7sp/tgbot-cpp.git /tgbot-cpp \
    && cd /tgbot-cpp \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && make install

# Создаем рабочую директорию
WORKDIR /app

# Копируем исходный код проекта в контейнер
COPY . .

# Создаем директорию для сборки
RUN mkdir build
WORKDIR /app/build

# Сборка проекта с C++17
RUN cmake -DCMAKE_CXX_STANDARD=17 .. \
    && make

# Указываем команду для запуска приложения
CMD ["./calculator_tg_bot"]
