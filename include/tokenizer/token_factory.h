//
// Created by Pew on 15-04-2023.
//

#pragma once

#include <string_view>

#include "../common.h"
#include "./token.h"

NAMESPACE_BEGIN
namespace lexer {


class TokenFactory {
public:
  // Creates a token with the given symbol, identifier type, and expression type
  static Token create_token(std::string_view symbol, std::string_view identifier_type, std::string_view expression_type,
                            unsigned int line, unsigned int column);

private:
  // Helper methods to convert string_view to Symbol, IdentifierType, and ExpressionType
  static Symbol string_to_symbol(std::string_view symbol_str);
  static IdentifierType string_to_identifier_type(std::string_view identifier_type_str);
  static ExpressionType string_to_expression_type(std::string_view expression_type_str);
};


}  // namespace lexer
NAMESPACE_END
