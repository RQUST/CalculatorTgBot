set(TOKENIZE_SRC ${CMAKE_SOURCE_DIR}/calculator/tokenize/tokenize.cpp)

add_executable(tokenize_public_test ${TOKENIZE_SRC} tokenize_public_test.cpp)