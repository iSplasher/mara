//
// Created by Pew on 15-04-2023.
//

#include "../include/tokenizer/token_factory.h"

#include <stdexcept>

using namespace NAMESPACE::lexer;

Token TokenFactory::create_token(std::string_view symbol, std::string_view identifier_type, std::string_view expression_type,
                                 unsigned int line, unsigned int column) {
    Token token;
    token.location = {line, column, line, column};
    token.symbol = string_to_symbol(symbol);
    token.identifier_type = string_to_identifier_type(identifier_type);
    token.expression_type = string_to_expression_type(expression_type);
    return token;
}

Symbol TokenFactory::string_to_symbol(std::string_view symbol_str) {
    // Implement the conversion from string_view to Symbol enum
    // Example:
    // if (symbol_str == "decl_keyword") {
    //   return Symbol::decl_keyword;
    // }
}

IdentifierType TokenFactory::string_to_identifier_type(std::string_view identifier_type_str) {
    // Implement the conversion from string_view to IdentifierType enum
    // Example:
    // if (identifier_type_str == "identifier") {
    //   return IdentifierType::identifier;
    // }
}

ExpressionType TokenFactory::string_to_expression_type(std::string_view expression_type_str) {
    // Implement the conversion from string_view to ExpressionType enum
    // Example:
    // if (expression_type_str == "declaration") {
    //   return ExpressionType::declaration;
    // }
}
