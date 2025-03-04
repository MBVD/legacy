#pragma once

#include "ast.hpp"

#include "lexer.hpp"
#include "parser.hpp"

#include <unordered_map>
#include <cmath>
#include <functional>
#include <stdexcept>

class Expression {
public:
    Expression(const std::string&);
    Expression(ASTNode* root);

    std::string to_string() const;
    double eval(const std::unordered_map<std::string, double>&) const;
    Expression& derivative(std::string);
private:
    ASTNode *root;
    std::string input;

    std::string to_string_helper(ASTNode*) const;
    double eval_helper(ASTNode*, const std::unordered_map<std::string, double>&) const;
    ASTNode* derivative_helper(ASTNode*, std::string) const;


    static const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>> builtins;

    static const std::unordered_map<std::string, double> constants;
};