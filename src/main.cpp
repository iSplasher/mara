
#include "../include/lexer.h"
#include "../include/tree.h"

using namespace rara;

int main( int argc, const char** argv ) {

  std::string  program = "a :: 2";
  lexer::Lexer lexer( program );

  Tree<int> tree( 0 );

  auto tokens = lexer.tokenize();

  return 0;
}
