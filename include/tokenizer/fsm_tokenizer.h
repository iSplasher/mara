//
// Created by Pew on 15-04-2023.
//

#pragma once

#include <map>
#include <memory>
#include <parallel_hashmap/phmap.h>
#include <string>
#include <vector>

#include "./tokenizer.h"

NAMESPACE_BEGIN
namespace lexer {

class FSM {
public:
  FSM(const std::string_view name, const std::string_view pattern);

  // move constructor
  FSM(FSM&& other) noexcept
        : name(std::move(other.name)), pattern(std::move(other.pattern)) {}

  // copy constructor
  FSM(const FSM& other) = default;



  // Other FSM-related methods and members

private:
  std::string_view name;
  std::string_view pattern;
};

class FSMTokenizer : public Tokenizer {
public:
  FSMTokenizer(const std::string_view program);

  // Adds an FSM for a specific token type
  void add_fsm(const std::string_view token_name, const std::string_view pattern);

  // Attempts to match an FSM to the current input
  std::pair<bool, std::string_view> match_fsm(const std::string_view input);

protected:
  // Overrides the base class method to handle tokens using FSMs
  virtual void handle_token(char c, std::vector<Token>& tokens) override;

private:
  phmap::flat_hash_map<std::string_view, FSM> fsm_map;
};


}  // namespace lexer
NAMESPACE_END
