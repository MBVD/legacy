#include "ast.hpp"
#include "visitor.hpp"

BinaryNode::BinaryNode(const std::string& op, const node& left, const node& right) : op(op), left(left), right(right) {}

UnaryNode::UnaryNode(const std::string& op, const node& base) : op(op), base(base) {}

FunctionNode::FunctionNode(const std::string& name, const std::vector<node>& args) : name(name), args(args) {}

ParenthesizedNode::ParenthesizedNode(const node& base) : base(base) {}

NumberNode::NumberNode(double value) : value(value) {}

VarNode::VarNode(const std::string& name) : name(name) {}

void BinaryNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void UnaryNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void FunctionNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void ParenthesizedNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void NumberNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void VarNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}