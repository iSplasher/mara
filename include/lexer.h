#pragma once

#include <string>
#include <vector>

#include "./common.h"

/**
 * @brief Represents any kind of bult-in token made up
 *       of text or symbols. These do not have whitespace in between.
 */
enum class Symbol {
  any            = 0xF,  // 28bits = 0xFFFFFFFF,
  none           = 0x0,

  /////// Keywords

  // declaration with :: or 'as'
  decl_keyword   = 1 << 0,
  // return from function
  return_keyword = 1 << 1,
};

DEFINE_ENUM_FLAGS( Symbol )

/**
 * @brief Represents the type of characters in the source code.
 *
 */
enum class IdentifierType {
  any        = 0xF,  // 28bits = 0xFFFFFFFF,
  none       = 0x0,
  // indetifier that refers to variable/function names, etc
  identifier = 1 << 0,
  // identifier that refers to a type
  type       = 1 << 1,
  // identifier that refers to a keyword or symbol like ::
  token      = 1 << 2,
  // identifier that refers to a [...] or :(): or (..) this differs
  // from symbol in that it allows for content inside the symbols
  value      = 1 << 3,

  // identifier that refers to a literal like true, 2, 3.14
  literal    = 1 << 4,
};

DEFINE_ENUM_FLAGS( IdentifierType )

/**
 * @brief Represents the type of expression
 *
 */
enum class ExpressionType {
  any         = 0xF,  // 28bits = 0xFFFFFFFF,
  none        = 0x0,

  // a :: 2 or a :: (b: int, c: int) (...)
  declaration = 1 << 0,
  // a = 2!
  assignment  = 1 << 1,
};

DEFINE_ENUM_FLAGS( ExpressionType )

/**
 * @brief Represents built-in values types
 *
 */
enum class ValueType {
  any       = 0xF,  // 28bits = 0xFFFFFFFF,
  none      = 0x0,

  function  = 1 << 0,
  number    = 1 << 1,
  character = 1 << 2,
  string    = 1 << 3,
  list      = 1 << 4,
  object    = 1 << 5,
};

DEFINE_ENUM_FLAGS( ValueType )

struct Token {

  unsigned int line              = 0;  // line number of the token
  unsigned int column            = 0;  // column number of the token

  Symbol         symbol          = Symbol::none;
  IdentifierType identifier_type = IdentifierType::none;
  ExpressionType expression_type = ExpressionType::none;
};

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
