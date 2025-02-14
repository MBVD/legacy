#pragma once

#include <unordered_map>
#include <string>

#include "ast.hpp"
#include "visitor.hpp"

class Evaluator : public Visitor {
public:
    double evaluate(const node&, const std::unordered_map<std::string, double>&);

    void visit(BinaryNode&) override;
    void visit(UnaryNode&) override;
    void visit(FunctionNode&) override;
    void visit(ParenthesizedNode&) override;
    void visit(NumberNode&) override;
    void visit(VarNode&) override;

private:
    double result;
    std::unordered_map<std::string, double> vars;
};