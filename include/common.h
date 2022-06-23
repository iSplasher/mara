#pragma once

#include <type_traits>

// Allows scoped enum bitwise operations

#define DEFINE_ENUM_FLAGS(E)                                                                                                                                                \
    inline E operator+(E x, std::underlying_type_t<E> y) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(x) + static_cast<std::underlying_type_t<E>>(y)); }; \
    inline E operator&(E x, E y) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(x) & static_cast<std::underlying_type_t<E>>(y)); };                         \
    inline E operator|(E x, E y) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(x) | static_cast<std::underlying_type_t<E>>(y)); };                         \
    inline E operator^(E x, E y) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(x) ^ static_cast<std::underlying_type_t<E>>(y)); };                         \
    inline E operator~(E x) { return static_cast<E>(~static_cast<std::underlying_type_t<E>>(x)); };                                                                         \
    inline E &operator&=(E &x, E y)                                                                                                                                         \
    {                                                                                                                                                                       \
        x = x & y;                                                                                                                                                          \
        return x;                                                                                                                                                           \
    };                                                                                                                                                                      \
    inline E &operator|=(E &x, E y)                                                                                                                                         \
    {                                                                                                                                                                       \
        x = x | y;                                                                                                                                                          \
        return x;                                                                                                                                                           \
    };                                                                                                                                                                      \
    inline E &operator^=(E &x, E y)                                                                                                                                         \
    {                                                                                                                                                                       \
        x = x ^ y;                                                                                                                                                          \
        return x;                                                                                                                                                           \
    };                                                                                                                                                                      \
    inline bool flags(E x) { return static_cast<std::underlying_type_t<E>>(x) != 0; };
