//
// Created by Pew on 20-04-2023.
//
#include "include/test.h"
#include "../include/tokenizer/tokenizer.h"
#include "../include/utility.h"

using namespace NAMESPACE;


static test::suite _ = [] {


    using namespace test;
    using namespace test::spec;


    describe("tokenizer") = [] {

        it("should be able to handle an empty program") = [&] {
            auto t1 = lexer::Tokenizer{""};
            auto tokens = t1.tokenize();
            expect(tokens.size() == 0_i);
        };

        it("should tokenize a simple program") = [] {
            std::string_view program = "\"hello world\"";
            auto t1 = lexer::Tokenizer{program};

            auto r1 = lexer::TokenRule{"qouted string", "\"", "\""};
            t1.registerRule(r1);

            auto tokens = t1.tokenize();
            auto stack = t1.debugRuleStack();
            expect(stack.size() == 0_i);

            auto &char_history = t1._char_history;
            expect(_ul(char_history.size()) == _ul(program.size()));

            auto &stack_history = t1._rule_stack_history;
            expect(stack_history.size() == 1_i);
            auto r = stack_history[0];
            auto c = r->location.toString();
            auto ce = "(1:1)-(1:13)";
            expect(test::eq(c, ce));

            expect(tokens.size() == 0_i);
        };


        it("should work with backwards token rule") = [] {
            std::string_view program = "'hello world\"";
            auto t1 = lexer::Tokenizer{program};
            auto r1 = lexer::TokenRule{"qouted string", "\"", "'"};
            r1.direction = lexer::TokenDirection::LEFT;
            t1.registerRule(r1);

            auto tokens = t1.tokenize();
            auto stack = t1.debugRuleStack();
            expect(stack.size() == 0_i);

            auto &char_history = t1._char_history;
            expect(_ul(char_history.size()) == _ul(program.size() * 2 - 1));

            auto &stack_history = t1._rule_stack_history;
            expect(stack_history.size() == 1_i);
            auto r = stack_history[0];
            auto c = r->location.toString();
            auto ce = "(1:1)-(1:13)";
            expect(test::eq(c, ce));

            expect(tokens.size() == 0_i);
        };


        it("should be able to handle the identifier in a :: 2") = [] {
            std::string_view program = "a :: 2";
            auto t1 = lexer::Tokenizer{program};

            auto r1 = lexer::TokenRule{"identifier", "", " "};
            r1.matcher = [](char c, unsigned long idx, std::string_view program) {
                auto indices = contegiousText(program, idx);
                if (indices.first != idx) {
                    if (std::isdigit(c)) {
                        return true;
                    }
                }
                return std::isalpha(c) || c == '_';
            };

            t1.registerRule(r1);

            auto tokens = t1.tokenize();
            auto stack = t1.debugRuleStack();
            expect(stack.size() == 0_i);

            auto &char_history = t1._char_history;
            expect(_ul(char_history.size()) == _ul(program.size()));

            auto &stack_history = t1._rule_stack_history;
            expect(stack_history.size() == 1_i);
            auto r = stack_history[0];
            auto c = r->location.toString();
            auto ce = "(1:1)-(1:2)";
            expect(test::eq(c, ce));

            expect(tokens.size() == 0_i);
        };
    };

};
