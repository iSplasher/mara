//
// Created by Pew on 15-04-2023.
//

#pragma once

#include <format>
#include <parallel_hashmap/phmap_utils.h>
#include <string>
#include <string_view>

#include "../common.h"
#include "../logger.h"

NAMESPACE_BEGIN

namespace lexer {

/**
 * @brief Represents any kind of bult-in token made up
 *       of text or symbols. These do not have whitespace in between.
 */
enum class Symbol {
    any = 0xF,  // 28bits = 0xFFFFFFFF,
    none = 0x0,

    /////// Keywords

    // declaration with :: or 'as'
    decl_keyword = 1 << 0,
    // return from function
    return_keyword = 1 << 1,
};

DEFINE_ENUM_FLAGS(Symbol)

template<>
struct std::formatter<Symbol> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(Symbol v, FormatContext &ctx) {
        std::string name = "Symbol::";
        switch (v) {
            case Symbol::any:
                name += "any";
                break;
            case Symbol::none:
                name += "none";
                break;
            default:
                name += "<unknown>";
                break;
        }
        return formatter<std::string_view>::format(name, ctx);
    }
};

/**
 * @brief Represents the type of characters in the source code.
 *
 */
enum class IdentifierType {
    any = 0xF,  // 28bits = 0xFFFFFFFF,
    none = 0x0,
    // indetifier that refers to variable/function names, etc
    identifier = 1 << 0,
    // identifier that refers to a type
    type = 1 << 1,
    // identifier that refers to a keyword or symbol like ::
    token = 1 << 2,
    // identifier that refers to a [...] or :(): or (..) this differs
    // from symbol in that it allows for content inside the symbols
    value = 1 << 3,

    // identifier that refers to a literal like true, 2, 3.14
    literal = 1 << 4,
};

DEFINE_ENUM_FLAGS(IdentifierType)

template<>
struct std::formatter<IdentifierType> : formatter<string_view> {
    template<typename FormatContext>
    auto format(IdentifierType v, FormatContext &ctx) {
        std::string name = "Symbol::";
        switch (v) {
            case IdentifierType::any:
                name += "any";
                break;
            case IdentifierType::none:
                name += "none";
                break;
            default:
                name += "<unknown>";
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

/**
 * @brief Represents built-in values types
 *
 */
enum class ValueType {
    any = 0xF,  // 28bits = 0xFFFFFFFF,
    none = 0x0,

    function = 1 << 0,
    number = 1 << 1,
    character = 1 << 2,
    string = 1 << 3,
    list = 1 << 4,
    object = 1 << 5,
};

DEFINE_ENUM_FLAGS(ValueType)

/**
 * @brief Represents the type of expression
 *
 */
enum class ExpressionType {
    any = 0xF,  // 28bits = 0xFFFFFFFF,
    none = 0x0,

    // a :: 2 or a :: (b: int, c: int) (...)
    declaration = 1 << 0,
    // a = 2!
    assignment = 1 << 1,
};

DEFINE_ENUM_FLAGS(ExpressionType)

struct CodeLocation {
    unsigned int line_start = 0;  // line number of the token
    unsigned int column_start = 0;  // column number of the token
    unsigned int line_end = 0;  // line number of the token
    unsigned int column_end = 0;  // column number of the token

    [[nodiscard]] std::string toString() const {
        return std::format("({}:{})-({}:{})", line_start, column_start, line_end, column_end);
    }

    explicit operator bool() const noexcept {
        // check if line and column are valid
        if (line_start > line_end) {
            return false;
        }

        if (line_start == line_end && column_start > column_end) {
            return false;
        }

        return true;
    }


    bool operator==(const CodeLocation &rhs) const {
        return line_start == rhs.line_start && column_start == rhs.column_start && line_end == rhs.line_end && column_end == rhs.column_end;
    }

    /**
    * @brief This allows use of flat_hash_set or flat_hash_map
    *
    * @param t
    * @return size_t
    */
    friend size_t hash_value(const CodeLocation &t) {
        return phmap::HashState().combine(
                0, t.line_start, t.column_start, t.line_end, t.column_end
        );
    }

};


struct Token {

    CodeLocation location;

    Symbol symbol = Symbol::none;
    IdentifierType identifier_type = IdentifierType::none;
    ExpressionType expression_type = ExpressionType::none;

    /**
     * @brief Returns true if the token is valid
     *
     * @return true
     * @return false
     */
    [[nodiscard]] bool isValid() const {
        if (symbol == Symbol::none && identifier_type == IdentifierType::none && expression_type == ExpressionType::none) {
            logger::error("Invalid token: symbol, identifier_type and expression_type are all none");
            return false;
        }

        // check if code loc is valid
        if (!location) {
            logger::error("Invalid token: code location is invalid");
            return false;
        }

        return true;
    }

    /**
     * @brief Returns true if the token is at the start of the file
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    bool isStart() const {
        return location.line_start == 0 && location.column_start == 0 && location.line_end == 0 && location.column_end == 0;
    }

    bool operator==(const Token &other) const {
        return location == other.location && symbol == other.symbol &&
               identifier_type == other.identifier_type && expression_type == other.expression_type;
    };

    /**
     * @brief This allows use of flat_hash_set or flat_hash_map
     *
     * @param t
     * @return size_t
     */
    friend size_t hash_value(const Token &t) {
        return phmap::HashState().combine(
                0, t.location, t.symbol, t.identifier_type, t.expression_type
        );
    }
};

}  // namespace lexer

NAMESPACE_END