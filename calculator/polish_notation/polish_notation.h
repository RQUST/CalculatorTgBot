#ifndef POLISH_NOTATION_H
#define POLISH_NOTATION_H

#include <string_view>
#include <string>
#include <stdexcept>

class UnknownSymbolError : public std::runtime_error {
 public:
  explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
  }
};

class WrongExpressionError : public std::runtime_error {
 public:
  explicit WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {
  }
};

int CalculatePolishNotation(std::string_view input);

#endif  // POLISH_NOTATION_H