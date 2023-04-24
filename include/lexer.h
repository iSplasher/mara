#pragma once

#include <format>
#include <parallel_hashmap/phmap_utils.h>
#include <string>
#include <vector>

#include "./common.h"
#include "./tokenizer/tokenizer.h"

NAMESPACE_BEGIN

namespace lexer {



/**
 * @brief Lexer class that tokenizes the source code.
 *
 */
class Lexer {

public:
  Lexer( std::string program );

  /**
   * @brief Tokenizes the program and returns a vector of tokens.
   */
  std::vector<Token> tokenize();

private:
  std::string program;
};

}  // namespace lexer

NAMESPACE_END