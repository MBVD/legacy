#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "token.hpp"
#include "ast.hpp"

class Parser {
public:
    Parser(const std::vector<Token>&);

    node parse();
private:

    node parse_binary_expression(int = 0);
    node parse_unary_expression();
    node parse_primary_expression(); // макс. приор.
    node parse_function_expression();
    node parse_parenthesize_expression();

    static const std::unordered_map<std::string, int> precedences;

    std::size_t offset = 0; 
    std::vector<Token> tokens;
};
