#include <unordered_map>
#include <string>
#include <cmath>

#include "evaluator.hpp"

double Evaluator::evaluate(const node& root, const std::unordered_map<std::string, double>& vars) {
    this->vars = vars;
    root->accept(*this);
    return result;
}

void Evaluator::visit(BinaryNode& nd) {
    nd.left->accept(*this);
    auto lhs = result;
    nd.right->accept(*this);
    auto rhs = result;

    if (nd.op == "+") {
        result = lhs + rhs;
    } else if (nd.op == "-") {
        result = lhs - rhs;
    } else if (nd.op == "*") {
        result = lhs * rhs;
    } else if (nd.op == "/") {
        result = lhs / rhs;
    }
}

void Evaluator::visit(UnaryNode& nd) {
    nd.base->accept(*this);
    auto base = result;

    if (nd.op == "-") {
        result = -base;
    }
}

void Evaluator::visit(FunctionNode& nd) {
    std::vector<double> args;

    for (const auto& arg : nd.args) {
        arg->accept(*this);
        args.push_back(result);
    }

    if (nd.name == "sin") {
        result = std::sin(args[0]);
    } else if (nd.name == "cos") {
        result = std::cos(args[0]);
    } else if (nd.name == "log") {
        result = std::log(args[0]);
    } else if (nd.name == "pow") {
        result = std::pow(args[0], args[1]);
    }
}

void Evaluator::visit(ParenthesizedNode& nd) {
    nd.base->accept(*this);
}

void Evaluator::visit(NumberNode& nd) {
    result = nd.value;
}

void Evaluator::visit(VarNode& nd) {
    result = vars.at(nd.name);
}