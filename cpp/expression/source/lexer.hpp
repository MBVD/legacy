#pragma once
#include "token.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(const std::string&);

    std::vector<Token> tokenize();
private:
    std::string input;
    std::string::size_type index = 0;

    static const std::string metachars;
    static const std::unordered_map<std::string, TokenType> operators;

    Token extract();
    Token extract_number();
    Token extract_identifier();
    Token extract_operator();
};