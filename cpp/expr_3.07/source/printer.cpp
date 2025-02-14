#include <iostream>

#include "printer.hpp"

void Printer::print(const node& root) {
    root->accept(*this);
    std::cout << std::endl;
}

void Printer::visit(BinaryNode& nd) {
    nd.left->accept(*this);
    std::cout << ' ' << nd.op << ' ';
    nd.right->accept(*this);
}

void Printer::visit(UnaryNode& nd) {
    std::cout << nd.op;
    nd.base->accept(*this);
}

void Printer::visit(FunctionNode& nd) {
    std::cout << nd.name << '(';
    for (std::size_t i = 0; i < nd.args.size(); ++i) {
        nd.args[i]->accept(*this);
        if (i != nd.args.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ')';
}

void Printer::visit(ParenthesizedNode& nd) {
    std::cout << '(';
    nd.base->accept(*this);
    std::cout << ')';
}

void Printer::visit(NumberNode& nd) {
    std::cout << nd.value;
    
}

void Printer::visit(VarNode& nd) {
    std::cout << nd.name;
}