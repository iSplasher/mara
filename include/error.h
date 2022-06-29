#pragma once

#include <bitset>
#include <concepts>
#include <fmt/core.h>
#include <iostream>
#include <string>

#include "./common.h"

NAMESPACE_BEGIN

// forward declarations

template<typename T>
struct BaseErrorCode;

// --------------------------------------------------------------------------------------------------------------------

/**
 * @brief Represents a single Error type
 *
 * @tparam C Class that defines the error codes, used to distinguish between different error classes.
 * @tparam T underlying value type, default is unsigned int. TODO: maybe change to uint_8?
 */
template<class C, typename T = unsigned>
requires std::unsigned_integral<T>
struct ErrorType {
  using CodeType  = C;
  using ValueType = T;

  constexpr ErrorType( const T value )
    : value( value ) {}

  constexpr ErrorType( const T value, const std::string default_message )
    : value( value )
    , message( default_message ) {}

  constexpr ErrorType( const ErrorType<C, T>& c )
    : value( c )
    , message( c.message ) {}

  constexpr ErrorType( const ErrorType<C, T>& c, const std::string message )
    : value( c )
    , message( message ) {}

  constexpr ErrorType( const std::string message )
    : value( 1 << 0 )
    , message( message ) {}

  explicit operator ValueType() const { return value; }

  explicit operator bool() const { return value; }

  const T     value;
  std::string message;
};

/**
 * @brief Represents a set of Error types (this is a type alias)
 *
 * @tparam C Class that defines the error codes, used to distinguish between different error classes.
 * @tparam T underlying value type, default is unsigned int. TODO: maybe change to uint_8?
 */
template<class C, typename T = unsigned>
using Error = const ErrorType<C, T>;

/**
 * @brief Base class for error codes
 *
 * @tparam T underlying value type, default is unsigned int
 */
template<typename T = unsigned>
struct BaseErrorCode {
  inline static Error<BaseErrorCode, T> any     = 0xF;
  inline static Error<BaseErrorCode, T> unknown = 1 << 0;
};

/**
 * @brief Base class for error codes (this is a type alias)
 *
 * @tparam T underlying value type, default is unsigned int
 */
template<typename T = unsigned>
using BaseError = Error<BaseErrorCode<T>, T>;

// This part here are all operator overloads for ErrorType

template<class C, typename T>
std::ostream& operator<<( std::ostream& stream, const ErrorType<C, T>& error ) {
  const auto val = fmt::format( "{}({})", std::bitset<sizeof( T )>( error.value ).to_string(), error.value );
  stream << error.message.size() ? fmt::format( "{}|{}", val, error.message ) : val;
  return stream;
};

template<class C, typename T>
constexpr inline bool flags( const ErrorType<C, T> x ) {
  return static_cast<ErrorType<C, T>>( x ) != 0;
};

template<class C, typename T>
constexpr inline const ErrorType<C, T> operator+( const ErrorType<C, T> x, T y ) {
  return static_cast<ErrorType<C, T>>( static_cast<T>( x ) + static_cast<T>( y ) );
};

template<class C, typename T>
constexpr inline const ErrorType<C, T> operator&( const ErrorType<C, T> x, const ErrorType<C, T> y ) {
  return static_cast<ErrorType<C, T>>( static_cast<T>( x ) & static_cast<T>( y ) );
};

// for BaseErrorCode<T>
template<class C, typename T>
constexpr inline const ErrorType<C, T> operator&( const BaseError<T> x, const ErrorType<C, T> y ) {
  return static_cast<ErrorType<C, T>>( static_cast<T>( x ) & static_cast<T>( y ) );
};

template<class C, typename T>
constexpr inline const ErrorType<C, T> operator|( const ErrorType<C, T> x, const ErrorType<C, T> y ) {
  return static_cast<ErrorType<C, T>>( static_cast<T>( x ) | static_cast<T>( y ) );
};

// for BaseErrorCode<T>
template<class C, typename T>
constexpr inline const ErrorType<C, T> operator|( const BaseError<T> x, const ErrorType<C, T> y ) {
  return static_cast<ErrorType<C, T>>( static_cast<T>( x ) | static_cast<T>( y ) );
};

template<class C, typename T>
constexpr inline const ErrorType<C, T> operator^( const ErrorType<C, T> x, const ErrorType<C, T> y ) {
  return static_cast<ErrorType<C, T>>( static_cast<T>( x ) ^ static_cast<T>( y ) );
};

template<class C, typename T>
constexpr inline const ErrorType<C, T> operator~( const ErrorType<C, T> x ) {
  return static_cast<ErrorType<C, T>>( ~static_cast<T>( x ) );
};

template<class C, typename T>
constexpr inline const ErrorType<C, T>& operator&=( const ErrorType<C, T>& x, const ErrorType<C, T> y ) {
  x = x & y;
  return x;
};

template<class C, typename T>
constexpr inline const ErrorType<C, T>& operator|=( const ErrorType<C, T>& x, const ErrorType<C, T> y ) {
  x = x | y;
  return x;
};

template<class C, typename T>
constexpr inline const ErrorType<C, T>& operator^=( const ErrorType<C, T>& x, const ErrorType<C, T> y ) {
  x = x ^ y;
  return x;
};

template<class C, typename T>
constexpr inline bool operator==( const ErrorType<C, T>& x, const ErrorType<C, T> y ) {
  return x.value == y.value;
};

// --------------------------------------------------------------------------------------------------------------------

/**
 * @brief Common and general error types
 *
 */
struct ErrorCode : BaseErrorCode<> {
  inline static Error<ErrorCode> test = 1 << 1;
};

// --------------------------------------------------------------------------------------------------------------------

/**
 * @brief A result class that can be used to optionally return errors from functions.
 *
 * @tparam Result Value to return if no error occured
 * @tparam E The error type.
 */
template<typename Result, class E>
requires std::is_base_of<
  Error<typename E::CodeType, typename E::ValueType>,
  E>::value struct maybe : public expected<Result, E> {

  constexpr maybe( Result&& value )
    : expected<Result, E>( std::move( value ) ) {}

  constexpr maybe( Result& value )
    : expected<Result, E>( value ) {}

  constexpr maybe( E&& error )
    : expected<Result, E>( tl::unexpect, std::move( error ) ) {}

  constexpr maybe( E& error )
    : expected<Result, E>( tl::unexpect, error ) {}
};

NAMESPACE_END
