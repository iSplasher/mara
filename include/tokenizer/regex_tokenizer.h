//
// Created by Pew on 15-04-2023.
//

#pragma once

#include <regex>

#include "./tokenizer.h"

NAMESPACE_BEGIN
namespace lexer {

class RegexTokenizer : public Tokenizer {
public:
  explicit RegexTokenizer(const std::string_view program);

protected:
  // Override handle_token to use regular expressions for tokenizing
  virtual void handle_token(char c, std::vector<Token>& tokens) override;

private:
  // Define your regular expressions for different tokens here
  std::regex keyword_regex{R"((decl_keyword|return_keyword))"};
  std::regex identifier_regex{R"([a-zA-Z_][a-zA-Z0-9_]*)"};
  std::regex number_regex{R"(\d+(\.\d+)?)"};
  // Add more regular expressions for other token types as needed
};


}  // namespace lexer
NAMESPACE_END
