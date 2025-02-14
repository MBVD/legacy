#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <cctype>

#include "token.hpp"
#include "lexer.hpp"

Lexer::Lexer(const std::string& input) : input(input) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    for (auto c = input[offset]; c; c = input[offset]) {
		if (std::isblank(c)) {
			++offset;
		} else if (std::isdigit(c) || c == '.') {
			tokens.push_back(extract_number());
		} else if (std::isalpha(c) || c == '_') {
			tokens.push_back(extract_identifier());
		} else if (metachars.contains(c)) {
			tokens.push_back(extract_operator());
		} else {
			switch (c) {
			case '(':
				tokens.push_back(Token{TokenType::LPAREN, "("}); break;
			case ')':
				tokens.push_back(Token{TokenType::RPAREN, ")"}); break;
			case ',':
				tokens.push_back(Token{TokenType::COMMA, ","}); break;
			default:
				throw std::runtime_error("Undefined character " + c);
			}
			++offset;
		}
    }
	tokens.push_back({TokenType::END, ""});
    return tokens;
}

Token Lexer::extract_number() {
	std::size_t size = 0;
	for (; std::isdigit(input[offset + size]); ++size);
	if (input[offset + size] == '.') {
		++size;
		for (; std::isdigit(input[offset + size]); ++size);

		if (size == 1) {
			throw std::runtime_error("Invalid number");
		}
	}
	auto num = input.substr(offset, size);
	offset += size;
	return {TokenType::NUMBER, num};
}
    
Token Lexer::extract_identifier() {
    std::size_t size = 0;
	for (; std::isalnum(input[offset + size]) or input[offset + size] == '_'; ++size);

	auto id = input.substr(offset, size);
	offset += size;
	return {TokenType::IDENTIFIER, id};
}

Token Lexer::extract_operator() {
	std::string op;
	for (; metachars.contains(input[offset]); ++offset) {
		op.push_back(input[offset]);

		if (not operators.contains(op)) {
			--offset;
			op.pop_back();
			break;
		}
	}

	return {TokenType::OPERATOR, op};
}

const std::string Lexer::metachars = "+-*/^";

const std::unordered_set<std::string> Lexer::operators = {
    "+", "-", "*", "/", "^"
};