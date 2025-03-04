#pragma once

#include "ast.hpp"
#include "token.hpp"

class Parser {
public:
    Parser(const std::vector<Token>&);

    ASTNode *parse();
private:
    std::vector<Token> tokens;
    std::size_t index = 0;

    ASTNode *parse_add();
    ASTNode *parse_mul();
    ASTNode *parse_pow();
    ASTNode *parse_unary();
    ASTNode *parse_base();
    ASTNode *parse_group();
    ASTNode *parse_func();

    void advance();

    Token peek() const;
    Token prev() const;

    bool check(TokenType) const;
    bool match(TokenType);

    void expect(TokenType, const std::string&);

    void report(const std::string&);
};