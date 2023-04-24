//
// Created by Pew on 15-04-2023.
//

#include "../include/tokenizer/regex_tokenizer.h"
#include "../include/tokenizer/token_factory.h"


using namespace NAMESPACE::lexer;

RegexTokenizer::RegexTokenizer(const std::string_view program)
    : Tokenizer(program) {
  // Initialize additional regular expressions for other token types as needed
}

void RegexTokenizer::handle_token(char c, std::vector<Token>& tokens) {
  // Convert the current character to a string
  std::string current_char(1, c);

  auto line = 0;
  auto column = 0;

  // Check if the current character matches any of the regular expressions
  if (std::regex_match(current_char, keyword_regex)) {
    tokens.push_back(TokenFactory::create_token(current_char, "keyword", "", line, column));
  } else if (std::regex_match(current_char, identifier_regex)) {
    tokens.push_back(TokenFactory::create_token(current_char, "identifier", "", line, column));
  } else if (std::regex_match(current_char, number_regex)) {
    tokens.push_back(TokenFactory::create_token(current_char, "number", "", line, column));
  }

  // Add more checks for other token types based on their regular expressions
}
