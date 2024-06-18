#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H


#include "../polish_notation/expressions.h"
#include "../tokenize/tokenize.h"
#include "parser.h"

#include <string_view>

int CalculateExpression(std::string_view input);

#endif  // CALCULATOR_CALCULATOR_H