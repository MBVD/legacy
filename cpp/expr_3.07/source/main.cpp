#include <iostream>
#include <string>

#include "expression.hpp"

int main() {
    std::string input;
    std::getline(std::cin, input);
    Expression expr(input);
    expr.print();

    auto result = expr.eval({ {"x", 2} });
    std::cout << "Result is " << result << std::endl;
    return 0;
}