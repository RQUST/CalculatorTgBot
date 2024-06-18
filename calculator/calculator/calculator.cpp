#include "calculator.h"

int CalculateExpression(std::string_view input) {
  // tokenize
  // ParseExpression
  // expression->Calculate()
  const auto tokens = Tokenize(input);
  size_t pos = 0;
  const auto expression = ParseExpression(tokens, pos);
  if (pos < tokens.size()) {
    throw WrongExpressionError("extra tokens detected");
  }
  return expression->Calculate();
}
