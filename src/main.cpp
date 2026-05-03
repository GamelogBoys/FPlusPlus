#include <fstream>
#include <sstream>
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "[F++ Internal] Usage: fpp --run <file.fpp>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[2]);
    if (!file.is_open()) {
        std::cerr << "[Error] Could not find or open " << argv[2] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    Interpreter interpreter;
    interpreter.execute(ast->statements);

    return 0;
}