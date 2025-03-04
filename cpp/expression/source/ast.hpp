#pragma once

#include <string>
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
};

/*
enum class OpType{
    BINARY_ADD, BINARY_SUB, BINARY_MUL, BINARY_DIV, BINARY_POW,
    UNARY_POS, UNARY_NEG
};*/


struct BinaryNode : ASTNode {
    std::string op;
    ASTNode *left, *right;
    //OpType op;

    BinaryNode(const std::string& op, ASTNode* left, ASTNode* right) : op(op), left(left), right(right) {}
};

struct UnaryNode : ASTNode {
    std::string op;
    ASTNode *base;
    //OpType op;

    UnaryNode(const std::string& op, ASTNode* base) : op(op), base(base) {}
};

struct NumberNode : ASTNode {
    double value;

    NumberNode(const std::string& value) : value(std::stod(value)) {}
};

struct VarNode : ASTNode {
    std::string name;

    VarNode(const std::string& name) : name(name) {}
};

struct FuncNode : ASTNode {
    std::string name;
    std::vector<ASTNode *> args;

    FuncNode(const std::string& name, const std::vector<ASTNode*>& args) : name(name), args(args) {}
};

struct GroupNode : ASTNode {
    ASTNode *base;

    GroupNode(ASTNode *base) : base(base) {}
};
