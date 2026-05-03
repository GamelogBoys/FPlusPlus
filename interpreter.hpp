#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "common.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <cmath> // Add this line at the top of the file

class Interpreter {
    std::map<std::string, std::string> memory;
    std::map<std::string, std::vector<std::unique_ptr<ASTNode>>*> functions;

private:
    // Helper to resolve variables and math
    std::string resolveValue(std::string raw) {
        if (raw.empty()) return "";
        
        // Trim spaces
        size_t first = raw.find_first_not_of(' ');
        size_t last = raw.find_last_not_of(' ');
        if (first != std::string::npos) raw = raw.substr(first, (last - first + 1));

        // String literals
        if (raw.size() >= 2 && raw.front() == '"' && raw.back() == '"') {
            return raw.substr(1, raw.size() - 2);
        }

        // Basic Math
        char ops[] = {'+', '-', '*', '/'};
        for (char op : ops) {
            size_t found = raw.find(op);
            if (found != std::string::npos) {
                std::string leftVal = resolveValue(raw.substr(0, found));
                std::string rightVal = resolveValue(raw.substr(found + 1));
                try {
                    float l = std::stof(leftVal);
                    float r = std::stof(rightVal);
                    float res = 0;
                    if (op == '+') res = l + r;
                    if (op == '-') res = l - r;
                    if (op == '*') res = l * r;
                    if (op == '/') res = (r != 0) ? l / r : 0;

                    return (std::fmod(res, 1.0f) == 0) ? std::to_string((int)res) : std::to_string(res);
                } catch (...) {
                    if (op == '+') return leftVal + rightVal; // String concat
                }
            }
        }

        if (memory.count(raw)) return memory[raw];
        return raw;
    }

    // Helper for logic comparisons
    bool evaluate(std::string l, std::string op, std::string r) {
        std::string vL = resolveValue(l);
        std::string vR = resolveValue(r);
        if (op == "==") return vL == vR;
        if (op == "!=") return vL != vR;
        try {
            float fL = std::stof(vL);
            float fR = std::stof(vR);
            if (op == ">") return fL > fR;
            if (op == "<") return fL < fR;
            if (op == ">=") return fL >= fR;
            if (op == "<=") return fL <= fR;
        } catch (...) {}
        return false;
    }

public:
    void execute(std::vector<std::unique_ptr<ASTNode>>& stmts) {
        for (auto& stmt : stmts) {
            if (auto var = dynamic_cast<VarDeclNode*>(stmt.get())) {
                if (var->isInput) {
                    std::cout << var->prompt;
                    std::string in; 
                    std::getline(std::cin, in);
                    memory[var->varName] = in;
                } else {
                    memory[var->varName] = resolveValue(var->value);
                }
            }
            else if (auto show = dynamic_cast<ShowNode*>(stmt.get())) {
                for (auto& c : show->contents) {
                    std::cout << resolveValue(c);
                }
                std::cout << std::endl;
            }
            else if (auto ifNode = dynamic_cast<IfNode*>(stmt.get())) {
                bool executed = false;
                for (auto& b : ifNode->branches) {
                    if (evaluate(b.left, b.op, b.right)) {
                        execute(b.body);
                        executed = true; 
                        break; 
                    }
                }
                if (!executed && !ifNode->elseBody.empty()) {
                    execute(ifNode->elseBody);
                }
            }
            else if (auto loop = dynamic_cast<WhenNode*>(stmt.get())) {
                while (evaluate(loop->left, loop->op, loop->right)) {
                    execute(loop->body);
                }
            }
            else if (auto funcDef = dynamic_cast<FuncDefNode*>(stmt.get())) {
                functions[funcDef->name] = &(funcDef->body);
            }
            else if (auto funcCall = dynamic_cast<FuncCallNode*>(stmt.get())) {
                if (functions.count(funcCall->name)) {
                    execute(*(functions[funcCall->name]));
                }
            }
        }
    }
};

#endif