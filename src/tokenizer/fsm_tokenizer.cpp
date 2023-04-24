//
// Created by Pew on 15-04-2023.
//


#include "../include/tokenizer/fsm_tokenizer.h"

using namespace NAMESPACE::lexer;

// FSM Implementation
FSM::FSM(const std::string_view name, const std::string_view pattern)
    : name(name), pattern(pattern) {
  // Implement FSM construction logic based on the pattern
}

// Other FSM-related methods and members


// FSMTokenizer Implementation
FSMTokenizer::FSMTokenizer(const std::string_view program) : Tokenizer(program) {}

void FSMTokenizer::add_fsm(const std::string_view token_name, const std::string_view pattern) {
  fsm_map.emplace(token_name, FSM(token_name, pattern));
}

std::pair<bool, std::string_view> FSMTokenizer::match_fsm(const std::string_view input) {
  for (const auto& entry : fsm_map) {
    const auto token_name = entry.first;
    const auto& fsm = entry.second;

    // Implement FSM matching logic using the input and the FSM instance
    // If there's a match, return true and the token_name

  }
  return {false, ""};
}

void FSMTokenizer::handle_token(char c, std::vector<Token>& tokens) {
  // Implement token handling logic using the FSMs in the fsm_map
  // Call match_fsm() and create tokens based on the matching FSMs
}