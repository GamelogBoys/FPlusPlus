#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

enum class TokenType {
    VAR, IDENTIFIER, EQUAL, STRING, NUMBER,
    SHOW, READ, IS, ORIS, ELSE, WHEN, DEFINE, BREAKLOOP,
    PLUS_EQ, MINUS_EQ, EQ_EQ, NOT_EQ, GT_EQ, LT_EQ,
    LBRACE, RBRACE, LPAREN, RPAREN, LBRACKET, RBRACKET, COLON, COMMA,
    AND, OR, NOT, EOF_TOKEN, HEADER_PART
};

struct Token {
    TokenType type;
    std::string value;
};

struct ASTNode { virtual ~ASTNode() = default; };

struct VarDeclNode : public ASTNode {
    std::string varName;
    std::string value;
    bool isInput = false;
    std::string prompt;
};

struct ShowNode : public ASTNode {
    std::vector<std::string> contents; 
};

struct IfNode : public ASTNode {
    struct Branch {
        std::string left, op, right;
        std::vector<std::unique_ptr<ASTNode>> body;
    };
    std::vector<Branch> branches;
    std::vector<std::unique_ptr<ASTNode>> elseBody;
};

struct WhenNode : public ASTNode {
    std::string left, op, right;
    std::vector<std::unique_ptr<ASTNode>> body;
};

// NEW: To store a function definition
struct FuncDefNode : public ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> body;
};

// NEW: To call a function later
struct FuncCallNode : public ASTNode {
    std::string name;
};

struct BlockNode : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> statements;
};

#endif