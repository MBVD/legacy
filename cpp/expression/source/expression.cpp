#include "expression.hpp"
#include<iostream>

Expression::Expression(ASTNode* root) {
    this->root = root;
    this->input = this->to_string();
}

Expression::Expression(const std::string& input) : input(input) {
    Lexer lexer(input);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    root = parser.parse();
}

std::string Expression::to_string() const {
    return to_string_helper(root);
}

double Expression::eval(const std::unordered_map<std::string, double>& vars) const {
    return eval_helper(root, vars);
}

std::string Expression::to_string_helper(ASTNode *root) const {
    if (auto node = dynamic_cast<BinaryNode*>(root); node) {
        return to_string_helper(node->left) + node->op + to_string_helper(node->right);
    }

    if (auto node = dynamic_cast<UnaryNode*>(root); node) {
        return node->op + to_string_helper(node->base);
    }

    if (auto node = dynamic_cast<GroupNode*>(root); node) {
        return "(" + to_string_helper(node->base) + ")";
    }

    if (auto node = dynamic_cast<FuncNode*>(root); node) {
        std::string res = node->name + "(";
        for (std::size_t i = 0; i < node->args.size(); ++i) {
            res += to_string_helper(node->args[i]);
            if (i < node->args.size() - 1) {
                res += ", ";
            }
        }
        return res + ")";
    }

    if (auto node = dynamic_cast<NumberNode*>(root); node) {
        return std::to_string(node->value);
    }

    if (auto node = dynamic_cast<VarNode*>(root); node) {
        return node->name;
    }
}


double Expression::eval_helper(ASTNode* root, const std::unordered_map<std::string, double>& vars) const {
    if (auto node = dynamic_cast<BinaryNode*>(root); node) {
        auto left = eval_helper(node->left, vars);
        auto right = eval_helper(node->right, vars);

        if (auto op = node->op; op == "+") {
            return left + right;
        } else if (op == "-") {
            return left - right;
        } else if (op == "*") {
            return left * right;
        } else if (op == "/") {
            return left / right;
        } else {
            return std::pow(left, right);
        }
    }

    if (auto node = dynamic_cast<UnaryNode*>(root); node) {
        if (node->op == "-") {
            return -eval_helper(node->base, vars);
        }
        return eval_helper(node->base, vars);
    }

    if (auto node = dynamic_cast<GroupNode*>(root); node) {
        return eval_helper(node->base, vars);
    }

    if (auto node = dynamic_cast<FuncNode*>(root); node) {

        std::vector<double> args;

        for (auto arg : node->args) {
            args.push_back(eval_helper(arg, vars));
        }

        if (builtins.contains(node->name)) {
            return builtins.at(node->name)(args);
        }

        throw std::runtime_error("Unknown function: " + node->name);  
    }

    if (auto node = dynamic_cast<NumberNode*>(root); node) {
        return node->value;
    }

    if (auto node = dynamic_cast<VarNode*>(root); node) {
        if (constants.contains(node->name)) {
            return constants.at(node->name);
        }
        if (vars.contains(node->name)) {
            return vars.at(node->name);
        }

        throw std::runtime_error("Unknown variable: " + node->name);
    }
}

const std::unordered_map<std::string, double> Expression::constants = {
    {"e", 2.71}, {"pi", 3.14}
};

const std::unordered_map<std::string, std::function<double(const std::vector<double>&)>> Expression::builtins = {
    {"sin", [] (const std::vector<double>& args) -> double {
        if (args.size() != 1) {
            throw std::runtime_error("Invalid number of arguments");
        }
        return std::sin(args[0]);
    }},
    {"cos", [] (const std::vector<double>& args) -> double {
        if (args.size() != 1) {
            throw std::runtime_error("Invalid number of arguments");
        }
        return std::cos(args[0]);
    }},
    {"log", [] (const std::vector<double>& args) -> double {
        if (args.size() != 1) {
            throw std::runtime_error("Invalid number of arguments");
        }
        return std::log(args[0]);
    }},
    {"pow", [] (const std::vector<double>& args) -> double {
        if (args.size() != 2) {
            throw std::runtime_error("Invalid number of arguments");
        }
        return std::pow(args[0], args[1]);
    }}
};

Expression& Expression::derivative(std::string var) {
    ASTNode* new_root = derivative_helper(this->root, var);
    Expression* expr = new Expression(new_root);
    return *expr;
}


ASTNode* Expression::derivative_helper(ASTNode* root, std::string var) const {
    if (auto tmp = dynamic_cast<NumberNode*>(root); tmp){
        return new NumberNode("0");
    }
    if (auto tmp = dynamic_cast<VarNode*>(root); tmp){
        if (tmp->name == var)
            return new NumberNode("1");
        return new NumberNode("0");
    }
    if (auto tmp = dynamic_cast<GroupNode*>(root); tmp) {
        return derivative_helper(tmp->base, var);
    }
    if (auto tmp = dynamic_cast<FuncNode*>(root); tmp) {
        if (builtins.contains(tmp->name)) {
            std::vector<ASTNode*> dargs;
            bool var_is_here = false;
            for (auto arg : tmp->args){
                auto darg = derivative_helper(arg, var);
                if (auto darg_num = dynamic_cast<NumberNode*>(darg); darg_num != nullptr){ // в функции нет переменной по которой мы дифф
                    if (darg_num -> value != 0)
                        var_is_here = true;  
                } else {
                    var_is_here = true;
                }
                dargs.push_back(darg);
            }
            if (!var_is_here) return new NumberNode("0");
            auto left = dargs[0];
            for (auto darg = dargs.begin()++; darg != dargs.end(); darg++){
                left = new BinaryNode("*", left, *darg);
            }
            if (tmp -> name == "sin"){
                auto cos = new FuncNode("cos", tmp->args);
                return new BinaryNode("*", cos, left);
            }
            if (tmp -> name == "cos"){
                auto sin = new FuncNode("sin", dargs);
                auto _sin = new UnaryNode("-", sin);
                return new BinaryNode("*", _sin, left);
            }
            if (tmp -> name == "ln"){
                auto dividend = new NumberNode("1");
                auto divider = dargs[0];
                auto dln = new BinaryNode("/", dividend, divider);
                return new BinaryNode("*", dln, left);
            }
            if (tmp -> name == "log"){
                auto divident = new NumberNode("1");
                auto lna = new FuncNode("ln", {tmp->args[0]}); // ln u
                auto divider = new GroupNode(new BinaryNode("*", tmp->args[1], lna));// v * lnu

                return new BinaryNode ("*", new BinaryNode("/", divident, divider), left); // 1/(vlnu) * v` u`
            }
            if (tmp -> name == "pow"){
                auto pow = new BinaryNode("^", tmp->args[0], tmp->args[1]);
                return derivative_helper(pow, var);
            }
        }
        return new FuncNode(tmp->name + "_" + var, tmp->args); // return just g`(x)
    }
    if (auto tmp = dynamic_cast<UnaryNode*>(root); tmp){
        return new UnaryNode(tmp -> op, derivative_helper(tmp->base, var));
    }
    if (auto tmp = dynamic_cast<BinaryNode*>(root); tmp){
        if (tmp -> op == "+" || tmp -> op == "-")
            return new BinaryNode(tmp->op, derivative_helper(tmp->left, var), derivative_helper(tmp->right, var));
        if (tmp -> op == "*"){
            auto left = derivative_helper(tmp->left, var);
            auto right = derivative_helper(tmp->right, var);
            left = new BinaryNode("*", left, tmp->right);
            right = new BinaryNode("*", tmp->left, right);
            return new BinaryNode("+", left, right);
        }
        if (tmp -> op == "/") {  
            auto left = derivative_helper(tmp->left, var);
            auto right = derivative_helper(tmp->right, var);
            left = new BinaryNode("*", left, tmp->right);
            right = new BinaryNode("*", tmp -> left, right);
            auto dividend = new BinaryNode("-", left, right);
            auto divider = new BinaryNode("^", tmp->right, new NumberNode("2"));
            return new BinaryNode("/", dividend, divider);
        }
        if (tmp -> op == "^"){
            auto du = derivative_helper(tmp->left, var);
            auto dv = derivative_helper(tmp->right, var);
            auto lnu = new FuncNode("ln", {tmp->left}); // ln(u)
            auto power = new BinaryNode("*", tmp->right, lnu); // v * ln(u)
            auto exp = new BinaryNode("^", new VarNode("e"), power);
            auto group_left = new BinaryNode("*", dv, lnu); 
            auto dividend = new BinaryNode("*", tmp->right, du);
            auto group_right = new BinaryNode("/", dividend, tmp->left); 

            auto group_expr = new BinaryNode("+", group_left, group_right);
            return new BinaryNode("*", exp, new GroupNode(group_expr)); 
        }
    }
}
