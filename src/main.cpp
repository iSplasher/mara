
#include "../include/lexer.h"
#include "../include/tree.h"
#include "../include/logger.h"

#include <iostream>
#include <format>

using namespace NAMESPACE;

int main(int argc, const char **argv) {

    setupLogger();

    std::string program = "a :: 2";
    lexer::Lexer lexer(program);

    Tree<int> tree(0);


    return 0;
}
