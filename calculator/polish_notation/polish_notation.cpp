#include "polish_notation.h"
#include "expressions.h"
#include "../tokenize/tokenize.h"

std::unique_ptr<IExpression> Parse(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }

  const auto& token = tokens[pos++];

  // если неизвестный токен
  if (std::holds_alternative<UnknownToken>(token)) {
    throw UnknownSymbolError("Unknown token: " + std::get<UnknownToken>(token).value);
  }

  // если )
  if (std::holds_alternative<ClosingBracketToken>(token)) {
    throw WrongExpressionError("No matching (");
  }

  // если (
  if (std::holds_alternative<OpeningBracketToken>(token)) {
    auto sub_expression = Parse(tokens, pos);
    if (pos >= tokens.size() || !std::holds_alternative<ClosingBracketToken>(tokens[pos++])) {
      throw WrongExpressionError("No matching )");
    }
    return sub_expression;
  }

  // если константа
  if (std::holds_alternative<NumberToken>(token)) {
    return std::make_unique<Constant>(std::get<NumberToken>(token).value);
  }

  // если унарная операция
  auto first_arg = Parse(tokens, pos);
  if (std::holds_alternative<SqrToken>(token)) {
    return std::make_unique<Square>(std::move(first_arg));
  }
  if (std::holds_alternative<AbsToken>(token)) {
    return std::make_unique<AbsoluteValue>(std::move(first_arg));
  }
  if (pos >= tokens.size() || std::holds_alternative<ClosingBracketToken>(tokens[pos])) {
    if (std::holds_alternative<PlusToken>(token)) {
      return std::make_unique<Plus>(std::move(first_arg));
    }
    if (std::holds_alternative<MinusToken>(token)) {
      return std::make_unique<Minus>(std::move(first_arg));
    }
  }

  // если бинарная операция
  auto second_arg = Parse(tokens, pos);
  if (std::holds_alternative<PlusToken>(token)) {
    return std::make_unique<Sum>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MinusToken>(token)) {
    return std::make_unique<Subtract>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MultiplyToken>(token)) {
    return std::make_unique<Multiply>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<ResidualToken>(token)) {
    return std::make_unique<Residual>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<DivideToken>(token)) {
    return std::make_unique<Divide>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MinToken>(token)) {
    return std::make_unique<Minimum>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MaxToken>(token)) {
    return std::make_unique<Maximum>(std::move(first_arg), std::move(second_arg));
  }

  throw UnknownSymbolError("Unknown token type");
}

int CalculatePolishNotation(std::string_view input) {
  const auto tokens = Tokenize(input);
  size_t pos = 0;
  const auto expression = Parse(tokens, pos);
  if (pos < tokens.size()) {  // прочитали не все токены (выражение составлено некорректно)
    throw WrongExpressionError("extra tokens detected");
  }
  return expression->Calculate();
}