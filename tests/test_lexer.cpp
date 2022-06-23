#include <catch2/catch_test_macros.hpp>
#include <string>

#include "../include/lexer.h"

int a = 2;

SCENARIO("Lexer can correctly tokenize", "[lexer]")
{
    GIVEN("a declaration")
    {
        std::string program = "a :: 2";
        Lexer lexer(program);
        WHEN("We tokenize the program")
        {
            auto tokens = lexer.tokenize();
            THEN("We get the correct number of tokens")
            {
                REQUIRE(tokens.size() == 6);
            }
            THEN("We get the correct identifiers")
            {
                REQUIRE(tokens.size() == 6);
            }
        }
    }
}