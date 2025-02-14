#include "expression.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "evaluator.hpp"

Expression::Expression(const std::string& input) {
    Lexer lexer(input);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    root = parser.parse();
}

void Expression::print() const {
    Printer printer;
    printer.print(root);
}

double Expression::eval(const std::unordered_map<std::string, double>& vars) const {
    Evaluator evaluator;

    return evaluator.evaluate(root, vars);
}