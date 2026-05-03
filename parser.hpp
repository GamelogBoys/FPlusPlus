#ifndef PARSER_HPP
#define PARSER_HPP
#include "common.hpp"

class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;
public:
    Parser(std::vector<Token> t) : tokens(t) {}

    std::unique_ptr<BlockNode> parseBlock() {
        auto block = std::make_unique<BlockNode>();
        while (pos < tokens.size() && tokens[pos].type != TokenType::RBRACE && tokens[pos].type != TokenType::EOF_TOKEN) {
            
            if (tokens[pos].type == TokenType::VAR) {
                auto node = std::make_unique<VarDeclNode>();
                pos++; // skip var

                // SKIP types like int, string, float
                if (pos < tokens.size() && (tokens[pos].value == "int" || tokens[pos].value == "string" || tokens[pos].value == "float")) {
                    pos++; 
                }

                if (pos < tokens.size()) node->varName = tokens[pos++].value;
                if (pos < tokens.size() && tokens[pos].type == TokenType::EQUAL) pos++; 
                
                if (pos < tokens.size() && tokens[pos].type == TokenType::READ) {
                    node->isInput = true; pos++; 
                    if (pos < tokens.size() && tokens[pos].type == TokenType::LBRACE) pos++;
                    if (pos < tokens.size()) node->prompt = tokens[pos++].value;
                    if (pos < tokens.size() && tokens[pos].type == TokenType::RBRACE) pos++;
                } else {
                    // Combine everything until next line/token to support math expressions
                    std::string val;
                    while(pos < tokens.size() && tokens[pos].type != TokenType::VAR && tokens[pos].type != TokenType::SHOW && tokens[pos].type != TokenType::IS && tokens[pos].type != TokenType::WHEN && tokens[pos].type != TokenType::RBRACE) {
                        val += tokens[pos++].value;
                    }
                    node->value = val;
                }
                block->statements.push_back(std::move(node));
            } 
            else if (tokens[pos].type == TokenType::SHOW) {
                auto node = std::make_unique<ShowNode>();
                pos++; 
                if (pos < tokens.size() && tokens[pos].type == TokenType::EQUAL) pos++;
                if (pos < tokens.size() && tokens[pos].type == TokenType::LBRACE) pos++;
                while (pos < tokens.size() && tokens[pos].type != TokenType::RBRACE) {
                    node->contents.push_back(tokens[pos].value); pos++;
                    if (pos < tokens.size() && tokens[pos].type == TokenType::COMMA) pos++;
                }
                if (pos < tokens.size() && tokens[pos].type == TokenType::RBRACE) pos++;
                block->statements.push_back(std::move(node));
            }
            else if (tokens[pos].type == TokenType::IS) {
                auto node = std::make_unique<IfNode>();
                while (pos < tokens.size() && (tokens[pos].type == TokenType::IS || tokens[pos].type == TokenType::ORIS)) {
                    pos++; 
                    IfNode::Branch b;
                    b.left = tokens[pos++].value;
                    b.op = tokens[pos++].value;
                    b.right = tokens[pos++].value;
                    if (pos < tokens.size() && tokens[pos].type == TokenType::LBRACE) pos++;
                    b.body = std::move(parseBlock()->statements);
                    if (pos < tokens.size() && tokens[pos].type == TokenType::RBRACE) pos++;
                    node->branches.push_back(std::move(b));
                }
                if (pos < tokens.size() && tokens[pos].type == TokenType::ELSE) {
                    pos++; 
                    if (pos < tokens.size() && tokens[pos].type == TokenType::LBRACE) pos++;
                    node->elseBody = std::move(parseBlock()->statements);
                    if (pos < tokens.size() && tokens[pos].type == TokenType::RBRACE) pos++;
                }
                block->statements.push_back(std::move(node));
            }
            else if (tokens[pos].type == TokenType::WHEN) {
                auto node = std::make_unique<WhenNode>();
                pos++; 
                node->left = tokens[pos++].value;
                node->op = tokens[pos++].value;
                node->right = tokens[pos++].value;
                if (pos < tokens.size() && tokens[pos].type == TokenType::LBRACE) pos++;
                node->body = std::move(parseBlock()->statements);
                if (pos < tokens.size() && tokens[pos].type == TokenType::RBRACE) pos++;
                block->statements.push_back(std::move(node));
            }
            else if (tokens[pos].type == TokenType::DEFINE) {
                auto node = std::make_unique<FuncDefNode>();
                pos++; 
                node->name = tokens[pos++].value;
                if (tokens[pos].type == TokenType::LPAREN) pos++;
                if (tokens[pos].type == TokenType::RPAREN) pos++;
                if (tokens[pos].type == TokenType::LBRACE) pos++;
                node->body = std::move(parseBlock()->statements);
                if (tokens[pos].type == TokenType::RBRACE) pos++;
                block->statements.push_back(std::move(node));
            }
            else if (tokens[pos].type == TokenType::IDENTIFIER) {
                if (pos + 1 < tokens.size() && tokens[pos+1].type == TokenType::LPAREN) {
                    auto node = std::make_unique<FuncCallNode>();
                    node->name = tokens[pos++].value;
                    pos += 2; 
                    block->statements.push_back(std::move(node));
                } else { pos++; }
            }
            else { pos++; }
        }
        return block;
    }

    std::unique_ptr<BlockNode> parse() {
        while (pos < tokens.size()) {
            TokenType t = tokens[pos].type;
            if (t == TokenType::VAR || t == TokenType::SHOW || t == TokenType::IS || t == TokenType::WHEN || t == TokenType::DEFINE) break;
            pos++;
        }
        return parseBlock();
    }
};
#endif