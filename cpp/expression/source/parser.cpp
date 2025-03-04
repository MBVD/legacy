#include "parser.hpp"

#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

ASTNode* Parser::parse() {
    return parse_add();
}

ASTNode* Parser::parse_add() {
    auto left = parse_mul();

    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        auto op = prev().value;
        auto right = parse_mul();
        left = new BinaryNode(op, left, right);
    }

    return left;
}

ASTNode* Parser::parse_mul() {
    auto left = parse_pow();

    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
        auto op = prev().value;
        auto right = parse_pow();
        left = new BinaryNode(op, left, right);
    }
    return left;
}

ASTNode* Parser::parse_pow() {
    auto left = parse_unary();

    if (match(TokenType::CARET)) {
        auto op = prev().value;
        auto right = parse_pow();
        left = new BinaryNode(op, left, right);
    }
    return left;
}

ASTNode* Parser::parse_unary() {
    if (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        auto op = prev().value;
        auto base = parse_unary();
        return new UnaryNode(op, base);
    }
    return parse_base();
}

ASTNode* Parser::parse_base() {
    if (match(TokenType::NUM)) {
        auto num = prev().value;
        return new NumberNode(num);
    }

    if (match(TokenType::LPAREN)) {
        return parse_group();
    }

    if (match(TokenType::ID)) {
        return parse_func();
    }

    report("Unexpected token: " + peek().value);

}

ASTNode* Parser::parse_group() {
    auto base = parse();
    expect(TokenType::RPAREN, "Expected ), got " + peek().value);
    return new GroupNode(base);
}

ASTNode* Parser::parse_func() {
    auto id = prev().value;

    if (match(TokenType::LPAREN)) {
        std::vector<ASTNode*> args;

        if (!check(TokenType::RPAREN)) {
            do {
                args.push_back(parse());
            } while (match(TokenType::COMMA));
        }
        expect(TokenType::RPAREN, "Expected ), got " + peek().value);
        return new FuncNode(id, args);
    }

    return new VarNode(id);
}

inline void Parser::advance() {
    ++index;
}

inline Token Parser::peek() const {
    return tokens[index];
}

inline Token Parser::prev() const {
    return tokens[index - 1];
}

inline bool Parser::check(TokenType type) const {
    return peek() == type;
}

inline bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

inline void Parser::expect(TokenType type, const std::string& message) {
    if (!match(type)) {
        report(message);
    }
}

inline void Parser::report(const std::string& message) {
    throw std::runtime_error(message);
}