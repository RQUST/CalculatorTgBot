cmake_minimum_required(VERSION 3.16)
project(calculator_tg_bot)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(calculator_tg_bot main.cpp)

target_link_libraries(calculator_tg_bot
    PRIVATE
    TgBot
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::system
    ZLIB::ZLIB
    CURL::libcurl
    Threads::Threads
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g -Wall -Wextra -pedantic -Werror -Wno-self-assign-overloaded -Wno-missing-braces")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include_directories(.)
