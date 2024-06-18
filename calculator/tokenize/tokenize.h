#ifndef TOKENIZE_TOKENIZE_H
#define TOKENIZE_TOKENIZE_H

#include <vector>
#include <string_view>
#include <string>
#include <variant>

struct PlusToken {};
inline bool operator==(PlusToken, PlusToken) noexcept {
  return true;
}

struct MinusToken {};
inline bool operator==(MinusToken, MinusToken) noexcept {
  return true;
}

struct MultiplyToken {};
inline bool operator==(MultiplyToken, MultiplyToken) noexcept {
  return true;
}

struct DivideToken {};
inline bool operator==(DivideToken, DivideToken) noexcept {
  return true;
}

struct ResidualToken {};
inline bool operator==(ResidualToken, ResidualToken) noexcept {
  return true;
}

struct OpeningBracketToken {};
inline bool operator==(OpeningBracketToken, OpeningBracketToken) noexcept {
  return true;
}

struct ClosingBracketToken {};
inline bool operator==(ClosingBracketToken, ClosingBracketToken) noexcept {
  return true;
}

struct SqrToken {};
inline bool operator==(SqrToken, SqrToken) noexcept {
  return true;
}

struct MinToken {};
inline bool operator==(MinToken, MinToken) noexcept {
  return true;
}

struct MaxToken {};
inline bool operator==(MaxToken, MaxToken) noexcept {
  return true;
}

struct AbsToken {};
inline bool operator==(AbsToken, AbsToken) noexcept {
  return true;
}

struct NumberToken {
  int value;
  bool operator==(const NumberToken& other) const noexcept {
    return value == other.value;
  }
};

struct UnknownToken {
  std::string value;
  bool operator==(const UnknownToken& other) const noexcept {
    return value == other.value;
  }
};

using Token =
    std::variant<UnknownToken, PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken, OpeningBracketToken,
                 ClosingBracketToken, SqrToken, MinToken, MaxToken, AbsToken, NumberToken>;

std::vector<Token> Tokenize(std::string_view input);

#endif  // TOKENIZE_TOKENIZE_H
