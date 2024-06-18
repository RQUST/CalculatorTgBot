#include "tokenize.h"

#include <cctype>
#include <unordered_map>
#include <stdexcept>

static const std::unordered_map<char, Token> kSymbolToToken{
    {'+', PlusToken{}},     {'-', MinusToken{}},          {'*', MultiplyToken{}},      {'/', DivideToken{}},
    {'%', ResidualToken{}}, {'(', OpeningBracketToken{}}, {')', ClosingBracketToken{}}};

static const std::unordered_map<std::string_view, Token> kWordToToken{
    {"min", MinToken{}}, {"max", MaxToken{}}, {"abs", AbsToken{}}, {"sqr", SqrToken{}}};

static bool IsSpace(char symbol) {
  return std::isspace(static_cast<unsigned char>(symbol));
}

static bool IsDigit(char symbol) {
  return '0' <= symbol && symbol <= '9';
}

static bool IsLetter(char symbol) {
  return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z');
}

static NumberToken ParseNumber(std::string_view input, size_t& pos) {
  // size_t number_size = 0;
  int value = 0;
  while (IsDigit(input[pos])) {
    value = value * 10 + (input[pos] - '0');
    ++pos;
  }
  return {value};
}

static Token ParseWord(std::string_view input, size_t& pos) {
  size_t word_size = 0;
  while (IsLetter(input[pos])) {
    ++word_size;
    ++pos;
  }
  const auto word = input.substr(pos - word_size, word_size);
  if (auto it = kWordToToken.find(word); it != kWordToToken.end()) {
    return it->second;
  }
  return UnknownToken{std::string(word)};
}

std::vector<Token> Tokenize(std::string_view input) {
  std::vector<Token> tokens;
  size_t pos = 0;
  while (pos < input.size()) {
    const auto symbol = input[pos];
    if (IsSpace(symbol)) {
      ++pos;
      continue;
    }
    if (auto it = kSymbolToToken.find(symbol); it != kSymbolToToken.end()) {
      tokens.emplace_back(it->second);
      ++pos;
    } else if (IsDigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
    } else if (IsLetter(symbol)) {
      tokens.emplace_back(ParseWord(input, pos));
    } else {
      throw std::runtime_error("Unknown symbol");
    }
  }
  return tokens;
}