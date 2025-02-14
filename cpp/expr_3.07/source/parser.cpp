#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>

#include "token.hpp"
#include "ast.hpp"
#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

node Parser::parse() {
    return parse_binary_expression();
}

node Parser::parse_binary_expression(int min_precedence) {
    auto left = parse_unary_expression();

    for (auto op = tokens[offset].value; precedences.contains(op) && precedences.at(op) >= min_precedence; op = tokens[offset].value) {

        ++offset; //skip operator

        auto right = parse_binary_expression(precedences.at(op));

        left = std::make_shared<BinaryNode>(op, left, right);
    }

    return left;
}

node Parser::parse_unary_expression() {
    if (auto op = tokens[offset].value; op == "+" || op == "-") {
        ++offset; //sлip operator
        auto base = parse_unary_expression();
        return std::make_shared<UnaryNode>(op, base);
    }

    return parse_primary_expression();
} 

node Parser::parse_primary_expression() {
    if (auto token = tokens[offset]; token == TokenType::NUMBER) {
        auto num = std::atof(token.value.c_str());
        ++offset; //skip number
        return std::make_shared<NumberNode>(num);
    } else if (token == TokenType::IDENTIFIER) {
        if (tokens[offset + 1] == TokenType::LPAREN) {
            return parse_function_expression();
        }
        ++offset; //skip namew
        return std::make_shared<VarNode>(token.value);
    } else if (token == TokenType::LPAREN) {
        return parse_parenthesize_expression();
    } else {
        throw std::runtime_error("Unexpected token " + token.value);
    }
}

node Parser::parse_parenthesize_expression() {
    ++offset; // skip (
    auto base = parse();
    if (tokens[offset] != TokenType::RPAREN) {
        throw std::runtime_error("Unclosed parenthesis");
    }
    ++offset; //skip )
    return std::make_shared<ParenthesizedNode>(base);
}

node Parser::parse_function_expression() {
    auto name = tokens[offset].value;
    offset += 2; //skip name and (                               

    std::vector<node> args;
    if (tokens[offset] != TokenType::RPAREN) {
        do {
            auto arg = parse();
            args.push_back(arg);
            if (tokens[offset] == TokenType::COMMA) {
                ++offset; //skip ,
                continue;
            } else if (tokens[offset] != TokenType::RPAREN) {
                throw std::runtime_error("Unclosed function call");
            }
        } while (tokens[offset] != TokenType::RPAREN);
    }

    return std::make_shared<FunctionNode>(name, args);
}




const std::unordered_map<std::string, int> Parser::precedences = {
    {"+", 0}, {"-", 0},
    {"*", 1}, {"/", 1},
    {"^", 2}
};

/*
<expr> := <sum_expr>
<sum_expr> := <mult_expr> | <mult_expr> {"+" | "-"} <sum_expr>
<mult_expr> := <pow_expr> | <pow_expr> {"*" | "/"} <mult_expr>
<pow_expr> := <unary_expr> | <unary_expr> "^" <pow_expr>

<bin_expr> := <unary_expr> | <unary_expr> <bin_op> <bin_expr>

<unary_expr> := <primary_expr> | {"+" | "-"} <unary_expr>
<primary_expr> := <var_expr> | <num_expr> | <paren_expr> | <func_expr>
<var_expr> := <id>
<num_expr> := <num>
<paren_expr> := "(" <expr> ")"
<func_expr> := <id> "(" <expr> {, <expr>} ")"
*/