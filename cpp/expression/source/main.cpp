#include "expression.hpp"

#include <iostream>

int main() {
    std::string input = "x ^ (2*x^2)";
    Expression expr(input);
    std::cout << expr.to_string() << std::endl;
    std::unordered_map<std::string, double> vars = {{"x", 5}};
    std::cout << expr.eval(vars) << std::endl;
    Expression new_expr = expr.derivative("x");
    std::cout<<new_expr.to_string()<<"\n";
}