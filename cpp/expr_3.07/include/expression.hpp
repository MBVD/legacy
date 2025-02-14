#pragma once

#include <string>
#include <unordered_map>

#include "ast.hpp"

class Expression {
public:
    Expression(const std::string&);

    void print() const;
    double eval(const std::unordered_map<std::string, double>&) const;
private:
    node root;
};