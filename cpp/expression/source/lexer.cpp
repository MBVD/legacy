#include "lexer.hpp"

#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (index < input.size()) {
        tokens.push_back(extract());
    }
    tokens.push_back({TokenType::END, ""});
    return tokens;
}

Token Lexer::extract() {
    while (std::isspace(input[index])) ++index;

    if (index >= input.size()) {
        return {TokenType::END, ""};
    }


    if (std::isdigit(input[index])) {
        return extract_number();
    }

    if (std::isalpha(input[index]) || input[index] == '_') {
        return extract_identifier();
    }

    if (metachars.contains(input[index])) {
        return extract_operator();
    }

    throw std::runtime_error("Unexpected symbol " + input[index]);
}

Token Lexer::extract_number() {
    std::size_t size = 0;

    while (std::isdigit(input[index + size])) ++size;

    if (input[index + size] == '.') {
        ++size;
        while (std::isdigit(input[index + size])) ++size;
    }

    std::string value(input, index, size);
    index += size;
    return {TokenType::NUM, value};
}

Token Lexer::extract_identifier() {
    std::size_t size = 0;

    while (std::isalnum(input[index + size]) || input[index + size] == '_') ++size; 

    std::string name(input, index, size);
    index += size;
    return {TokenType::ID, name};
}

Token Lexer::extract_operator() {
    std::string op;
    while (metachars.contains(input[index])) {
        op += input[index];
        if (!operators.contains(op)) { 
            op.pop_back();
            break;
        }
        ++index;
    }
    return {operators.at(op), op};
}

const std::string Lexer::metachars = "+-*/^(),";

const std::unordered_map<std::string, TokenType> Lexer::operators = {
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::STAR},
    {"/", TokenType::SLASH},
    {"^", TokenType::CARET},
    {",", TokenType::COMMA},
    {"(", TokenType::LPAREN},
    {")", TokenType::RPAREN},
};