#ifndef CALCULATOR_PARSER_H
#define CALCULATOR_PARSER_H

#include "../polish_notation/expressions.h"
#include "../tokenize/tokenize.h"

#include <string>
#include <stdexcept>
#include <string_view>
#include <memory>

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

std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos);
std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos);
std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos);

#endif  // CALCULATOR_PARSER_H