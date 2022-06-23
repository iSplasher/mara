#include "../include/lexer.h"

int main(int argc, const char **argv)
{

    std::string program = "a :: 2";
    Lexer lexer(program);

    auto tokens = lexer.tokenize();

    return 0;
}