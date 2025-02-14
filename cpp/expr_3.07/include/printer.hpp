#pragma once

#include "ast.hpp"
#include "visitor.hpp"

class Printer : public Visitor {
public:
    void print(const node&);

    void visit(BinaryNode&) override;
    void visit(UnaryNode&) override;
    void visit(FunctionNode&) override;
    void visit(ParenthesizedNode&) override;
    void visit(NumberNode&) override;
    void visit(VarNode&) override;
};