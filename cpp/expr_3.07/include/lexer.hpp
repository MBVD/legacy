#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "token.hpp"

class Lexer {
public:
    Lexer(const std::string&);

    std::vector<Token> tokenize();
private:

    Token extract_number();
    Token extract_identifier();
    Token extract_operator();

    static const std::string metachars;
    static const std::unordered_set<std::string> operators;
    

    std::size_t offset = 0;
    std::string input;
};