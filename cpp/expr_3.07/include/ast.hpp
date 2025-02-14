#pragma once

#include <string>
#include <vector>
#include <memory>

class Visitor;

struct ASTNode {
    virtual ~ASTNode() = default;

    virtual void accept(Visitor&) = 0;
};

using node = std::shared_ptr<ASTNode>;

struct BinaryNode : public ASTNode {
    std::string op;
    node left, right;

    BinaryNode(const std::string&, const node&, const node&);

    void accept(Visitor&) override;
};


struct UnaryNode : public ASTNode {
    std::string op;
    node base;

    UnaryNode(const std::string&, const node&);

    void accept(Visitor&) override;
};

struct FunctionNode : public ASTNode {
    std::string name;
    std::vector<node> args;

    FunctionNode(const std::string&, const std::vector<node>&);

    void accept(Visitor&) override;
};

struct ParenthesizedNode : public ASTNode {
    node base;

    ParenthesizedNode(const node&);

    void accept(Visitor&) override;
};

struct NumberNode : public ASTNode {
    double value;  

    NumberNode(double);

    void accept(Visitor&) override;
};

struct VarNode : public ASTNode {
    std::string name;

    VarNode(const std::string&);

    void accept(Visitor&) override;
};