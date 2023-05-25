//
// Created by Pew on 15-04-2023.
//

#include "../include/tokenizer/tokenizer.h"
#include "../include/logger.h"

#include <memory>
#include <algorithm>


using namespace NAMESPACE::lexer;

Tokenizer::Tokenizer(const std::string_view program)
        : _program(program) {

    _program_size = _program.size();
}

std::vector<Token> Tokenizer::tokenize() {
    evaluate();
    return _tokens;
}

void Tokenizer::handle_whitespace(char c, std::vector<Token> &tokens) {
    // Default implementation: do nothing with whitespace
}

bool Tokenizer::is_comment_start(char c, size_t index) {
    // Implement the comment start detection logic
}

size_t Tokenizer::handle_comments(size_t index) {
    // Implement comment handling logic
    // Return the updated index after handling the comment
}

void Tokenizer::handle_token(char c, std::vector<Token> &tokens) {
    // Default implementation: create a simple Token instance
    // Override this method in derived classes for more complex token handling
    Token token;
    tokens.push_back(token);
}

void Tokenizer::handle_start() {
    _tokens.clear();


}

void Tokenizer::evaluate() {
    char c;
    _current_line = 1;
    _current_index = 0;

    while (_current_index < _program_size && _current_index >= 0) {
        c = _program[_current_index];


#ifdef ENV_TEST
        _char_history.push_back(c);

        logger::trace("Current char: [{}]({}:{})", c, _current_line, _current_column);
#endif

        if (c == '\n') {
            switch (_current_direction) {
                case TokenDirection::RIGHT:
                    _current_line++;
                    _prev_line_column = _current_column;
                    _current_column = 0;
                    break;
                case TokenDirection::LEFT:
                    _current_line--;
                    _current_column = _prev_line_column;
                    break;
            }

        } else {
            switch (_current_direction) {
                case TokenDirection::RIGHT:
                    _current_column++;
                    break;
                case TokenDirection::LEFT:
                    _current_column--;
                    break;
            }
        }

        // check if any rule on stack terminates on this character
        auto terminated_rules = terminateStackRules(c);

        if (terminated_rules.size() > 0) {
            logger::trace("Terminated rules [{}]({}:{}): {}", c, _current_line, _current_column, terminated_rules.size());
        }

        auto rules = getRulesForChar(c);

        if (rules.size() > 0) {
            logger::trace("Rules for char [{}]({}:{}): {}", c, _current_line, _current_column, rules.size());
        }

        // add all rules that aren't terminated
        for (auto &rule: rules) {
            auto terminated = std::find_if(terminated_rules.begin(), terminated_rules.end(),
                                           [&rule](const TokenRule &terminated) {
                                               return terminated.name == rule->name;
                                           });

            if (terminated == terminated_rules.end()) {
                pushRule(rule);
            }
        }

        // apply terminated rules
        for (auto &rule: terminated_rules) {
            applyRule(std::move(rule));
        }

        if (_current_direction == TokenDirection::LEFT && _current_index == 0) {
            break;
        }

        switch (_current_direction) {
            case TokenDirection::RIGHT:
                _current_index++;
                break;
            case TokenDirection::LEFT:
                _current_index--;
                break;
        }
    }
}

void Tokenizer::registerRule(TokenRule &rule) {
    logger::debug("Registering rule: {}", rule.name);
    _rules.push_back(std::make_shared<TokenRule>(rule));
}

std::vector<std::shared_ptr<TokenRule>> Tokenizer::getRulesForChar(char c) {
    std::vector<std::shared_ptr<TokenRule>> rules;

    for (auto &rule: _rules) {
        std::shared_ptr<TokenRule> r = nullptr;
        if (rule->symbol.empty()) {
            r = rule;
        } else if (rule->symbol[0] == c) {
            r = rule;
        }

        if (r != nullptr && r->matcher != nullptr) {
            if (!r->matcher(c, _current_index, _program)) {
                r = nullptr;
            }
        }

        if (r != nullptr) {
            rules.push_back(r);
        }
    }

    return std::move(rules);
}

std::vector<TokenRule> Tokenizer::terminateStackRules(char c) {
    std::vector<TokenRule> rules;

    while (!_rule_stack.empty()) {
        auto &rule = _rule_stack.top();

        if (rule.terminator[0] == c) {
            _rule_stack.pop();

            switch (rule.direction) {
                case TokenDirection::RIGHT:
                    rule.location.line_end = _current_line;
                    rule.location.column_end = _current_column;
                    break;
                case TokenDirection::LEFT:
                    rule.location.line_start = _current_line;
                    rule.location.column_start = _current_column;
                    break;
            }

            rules.push_back(rule);
        } else {
            break;
        }
    }

    return std::move(rules);
}

void Tokenizer::pushRule(std::shared_ptr<TokenRule> rule) {
    logger::debug("Pushing rule: {}", rule->name);


    _current_direction = rule->direction;

    CodeLocation location = {0, 0, 0, 0};

    switch (rule->direction) {
        case TokenDirection::RIGHT:
            location = {_current_line, _current_column, 0, 0};
            break;
        case TokenDirection::LEFT:
            location = {0, 0, _current_line, _current_column};
            break;
    }

    _rule_stack.push(rule->clone(location));

#ifdef ENV_TEST
    _rule_stack_history.push_back(&_rule_stack.top());
#endif

}

void Tokenizer::applyRule(TokenRule &&rule) {
    logger::debug("Applying rule: {}", rule.name);
}







