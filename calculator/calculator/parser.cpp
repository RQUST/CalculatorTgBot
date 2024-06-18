#include "parser.h"

std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  auto expression = ParseTerm(tokens, pos);  // expression = ((A+B)-C)+D
  while (pos < tokens.size()) {
    const auto& token = tokens[pos];
    if (std::holds_alternative<PlusToken>(token)) {
      expression = std::make_unique<Sum>(std::move(expression), ParseTerm(tokens, ++pos));
    } else if (std::holds_alternative<MinusToken>(token)) {
      expression = std::make_unique<Subtract>(std::move(expression), ParseTerm(tokens, ++pos));
    } else if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError("Unknown symbol: " + std::get<UnknownToken>(token).value);
    } else if (std::holds_alternative<ClosingBracketToken>(token)) {
      break;
    } else {
      throw WrongExpressionError("unexpected operation");
    }
  }
  return expression;
}

std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  auto expression = ParseFactor(tokens, pos);
  while (pos < tokens.size()) {
    const auto& token = tokens[pos];
    if (std::holds_alternative<MultiplyToken>(token)) {
      expression = std::make_unique<Multiply>(std::move(expression), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<DivideToken>(token)) {
      expression = std::make_unique<Divide>(std::move(expression), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<ResidualToken>(token)) {
      expression = std::make_unique<Residual>(std::move(expression), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError("Unknown symbol: " + std::get<UnknownToken>(token).value);
    } else {
      break;
    }
  }
  return expression;
}

std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  const auto& token = tokens[pos];

  if (std::holds_alternative<UnknownToken>(token)) {
    throw UnknownSymbolError("Unknown token: " + std::get<UnknownToken>(token).value);
  }

  if (std::holds_alternative<NumberToken>(token)) {
    ++pos;
    return std::make_unique<Constant>(std::get<NumberToken>(token).value);
  }
  if (std::holds_alternative<ClosingBracketToken>(token)) {
    throw WrongExpressionError("No matching (");
  }
  if (std::holds_alternative<OpeningBracketToken>(token)) {
    auto sub_expression = ParseExpression(tokens, ++pos);
    if (pos >= tokens.size() || !std::holds_alternative<ClosingBracketToken>(tokens[pos++])) {
      throw WrongExpressionError("No matching )");
    }
    return sub_expression;
  }
  if (std::holds_alternative<SqrToken>(token)) {
    return std::make_unique<Square>(ParseFactor(tokens, ++pos));
  }
  if (std::holds_alternative<AbsToken>(token)) {
    return std::make_unique<AbsoluteValue>(ParseFactor(tokens, ++pos));
  }
  if (std::holds_alternative<PlusToken>(token)) {
    return std::make_unique<Plus>(ParseFactor(tokens, ++pos));
  }
  if (std::holds_alternative<MinusToken>(token)) {
    return std::make_unique<Minus>(ParseFactor(tokens, ++pos));
  }
  return nullptr;
}
