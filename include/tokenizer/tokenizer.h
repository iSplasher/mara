//
// Created by Pew on 15-04-2023.
//

#pragma once


#include <parallel_hashmap/phmap_utils.h>
#include <string_view>
#include <vector>
#include <stack>
#include <string>
#include <memory>
#include <functional>

#include "../common.h"
#include "./token.h"

NAMESPACE_BEGIN
namespace lexer {

class FSMTokenizer;

class RegexTokenizer;

enum class TokenDirection {
    LEFT,
    RIGHT,
};

struct TokenRule {
    friend class Tokenizer;

    std::string_view name;
    std::string_view symbol;
    std::string_view terminator;

    TokenDirection direction = TokenDirection::RIGHT;

    CodeLocation location = {0, 0, 0, 0};

    std::function<bool(char, unsigned long, std::string_view &)> matcher = nullptr;

    TokenRule clone(CodeLocation location) {
        TokenRule rule;
        rule.name = name;
        rule.symbol = symbol;
        rule.terminator = terminator;
        rule.direction = direction;
        rule.location = location;
        return rule;
    }

};

/**
 * @brief Base class for tokenizers.
 *
 */
class Tokenizer {
public:
    Tokenizer(std::string_view program);

    // Tokenizes the input program and returns a vector of tokens
    std::vector<Token> tokenize();

    void registerRule(TokenRule &rule);


protected:
    void handle_start();

    void evaluate();

    void pushRule(std::shared_ptr<TokenRule> rule);

    void applyRule(TokenRule &&rule);

    std::vector<std::shared_ptr<TokenRule>> getRulesForChar(char c);

    std::vector<TokenRule> terminateStackRules(char c);

    // Handles whitespace characters
    virtual void handle_whitespace(char c, std::vector<Token> &tokens);

    // Checks if a comment starts at the current character
    virtual bool is_comment_start(char c, size_t index);

    // Handles comments in the input program
    virtual size_t handle_comments(size_t index);

    // Handles token characters in the input program
    virtual void handle_token(char c, std::vector<Token> &tokens);


protected:
    Token _current_token;

private:
    std::string_view _program;
    std::vector<Token> _tokens;
    std::vector<std::shared_ptr<TokenRule>> _rules;

    std::stack<TokenRule> _rule_stack;

    TokenDirection _current_direction = TokenDirection::RIGHT;

    unsigned long _current_line = 0;
    unsigned long _current_column = 0;
    unsigned long _prev_line_column = 0;

    unsigned long _current_index = 0;
    unsigned long _program_size = 0;


#ifdef ENV_TEST
    public:

    std::stack<TokenRule> debugRuleStack() {
        auto rule_stack = std::stack(_rule_stack);
        return std::move(rule_stack);
    };

    std::vector<TokenRule*> _rule_stack_history;
    std::vector<char> _char_history;

#endif

};

}  // namespace lexer
NAMESPACE_END

