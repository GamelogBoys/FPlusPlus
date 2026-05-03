#ifndef LEXER_HPP
#define LEXER_HPP
#include "common.hpp"

class Lexer {
    std::string src;
    size_t pos = 0;
public:
    Lexer(std::string source) : src(source) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < src.length()) {
            char current = src[pos];
            if (isspace(current)) { pos++; continue; }
            if (current == '.') { 
                pos++; while (pos < src.length() && src[pos] != '.') pos++;
                pos++; continue;
            }
            if (current == '"') { 
                std::string res; pos++;
                while (pos < src.length() && src[pos] != '"') {
                    if (src[pos] == '\\') {
                        pos++;
                        if (src[pos] == 'n') res += '\n';
                        else res += src[pos];
                    } else res += src[pos];
                    pos++;
                }
                tokens.push_back({TokenType::STRING, res}); pos++; continue;
            }
            if (isalpha(current)) {
                std::string ident;
                while (pos < src.length() && (isalnum(src[pos]) || src[pos] == '_')) ident += src[pos++];
                if (ident == "var") tokens.push_back({TokenType::VAR, ident});
                else if (ident == "show") tokens.push_back({TokenType::SHOW, ident});
                else if (ident == "read") tokens.push_back({TokenType::READ, ident});
                else if (ident == "is") tokens.push_back({TokenType::IS, ident});
                else if (ident == "oris") tokens.push_back({TokenType::ORIS, ident});
                else if (ident == "else") tokens.push_back({TokenType::ELSE, ident});
                else if (ident == "when") tokens.push_back({TokenType::WHEN, ident});
                else if (ident == "define") tokens.push_back({TokenType::DEFINE, ident});
                else if (ident == "Par") tokens.push_back({TokenType::HEADER_PART, ident});
                else tokens.push_back({TokenType::IDENTIFIER, ident});
                continue;
            }
            
            std::string op2 = (pos + 1 < src.length()) ? src.substr(pos, 2) : "";
            if (op2 == "==") { tokens.push_back({TokenType::EQ_EQ, "=="}); pos += 2; continue; }
            if (op2 == "!=") { tokens.push_back({TokenType::NOT_EQ, "!="}); pos += 2; continue; }
            if (op2 == ">=") { tokens.push_back({TokenType::GT_EQ, ">="}); pos += 2; continue; }
            if (op2 == "<=") { tokens.push_back({TokenType::LT_EQ, "<="}); pos += 2; continue; }
            
            if (current == '=') tokens.push_back({TokenType::EQUAL, "="});
            else if (current == '>') tokens.push_back({TokenType::GT_EQ, ">"});
            else if (current == '<') tokens.push_back({TokenType::LT_EQ, "<"});
            else if (current == '{') tokens.push_back({TokenType::LBRACE, "{"});
            else if (current == '}') tokens.push_back({TokenType::RBRACE, "}"});
            else if (current == '(') tokens.push_back({TokenType::LPAREN, "("});
            else if (current == ')') tokens.push_back({TokenType::RPAREN, ")"});
            else if (current == ',') tokens.push_back({TokenType::COMMA, ","});
            // MATH OPERATORS ADDED BELOW
            else if (current == '+') tokens.push_back({TokenType::PLUS_EQ, "+"});
            else if (current == '-') tokens.push_back({TokenType::MINUS_EQ, "-"});
            else if (current == '*') tokens.push_back({TokenType::AND, "*"});
            else if (current == '/') tokens.push_back({TokenType::OR, "/"});
            
            pos++;
        }
        tokens.push_back({TokenType::EOF_TOKEN, ""});
        return tokens;
    }
};
#endif