#include "../include/lexer.h"

using namespace rara;
using namespace rara::lexer;

Lexer::Lexer( std::string program ) {
  this->program = program;
}

std::vector<Token> Lexer::tokenize() {
  auto v = std::vector<Token>();
  return v;
}