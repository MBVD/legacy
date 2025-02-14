#pragma once

#include "ast.hpp"

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(BinaryNode&) = 0;
    virtual void visit(UnaryNode&) = 0;
    virtual void visit(FunctionNode&) = 0;
    virtual void visit(ParenthesizedNode&) = 0;
    virtual void visit(NumberNode&) = 0;
    virtual void visit(VarNode&) = 0;
};