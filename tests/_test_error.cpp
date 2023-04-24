#include <catch2/catch_test_macros.hpp>
#include <tl/expected.hpp>
#include <type_traits>

#include "../include/error.h"

using namespace rara;

SCENARIO( "Error model", "[error]" ) {

  GIVEN( "Error" ) {

    WHEN( "Error" ) {

      struct TestErrorCode : BaseErrorCode<> {
        Error<TestErrorCode> error = { 1 << 1, "error" };
        Error<TestErrorCode> other = { 1 << 2, "other" };
        Error<TestErrorCode> alias = { 1 << 1, "alias" };
      };

      using TestError = Error<TestErrorCode>;
      static_assert( std::is_same_v<TestError, const ErrorType<TestErrorCode, unsigned int>>, "is not const" );
      static_assert( std::is_same_v<TestError::ValueType, unsigned int>, "is not value type" );
      static_assert( std::is_same_v<TestError::ValueType, bool> == false, "ValueType is bool" );
      static_assert( std::is_same_v<TestError::ValueType, int> == false, "ValueType is int" );
      static_assert( std::is_same_v<TestError::CodeType, int> == false, "CodeType is int" );
      static_assert( std::is_same_v<TestError::CodeType, TestErrorCode>, "CodeType is not given type" );

      TestErrorCode t;
      TestErrorCode t2;

      THEN( "error has 1 << 1 bit flag" ) {
        CHECK( static_cast<TestError::ValueType>( t.error ) == 1 << 1 );
      }

      THEN( "error == error" ) {
        CHECK( t.error == t2.error );
      }

      THEN( "error == alias" ) {
        CHECK( t.error == t2.alias );
      }

      THEN( "error != other_error" ) {
        CHECK( t.error != t.other );
      }

      THEN( "error default message" ) {
        CHECK( t.error.message == "error" );
      }

      THEN( "can override default message" ) {
        TestError e = { t.error, "override" };
        CHECK( e.message == "override" );
        CHECK( t.error.message == "error" );
      }

      THEN( "can instantiate only from code" ) {
        TestError e = { t.error };
        CHECK( e.message == "error" );
      }
    }

    WHEN( "Error 2" ) {

      struct TestErrorCode1 : BaseErrorCode<> {
        Error<TestErrorCode1> error = { 1 << 1, "error" };
      };

      using TestError1 = Error<TestErrorCode1>;

      struct TestErrorCode2 : BaseErrorCode<> {
        Error<TestErrorCode2> error = { 1 << 1, "error" };
      };

      using TestError2 = Error<TestErrorCode2>;

      static_assert( std::is_same_v<TestError1, TestError2> == false, "type shouldnt be the same" );

      TestErrorCode1 t1;
      TestErrorCode2 t2;

      THEN( "error1.value != error2.value where value have the same integer" ) {
        CHECK( std::is_same_v<TestError1, TestError2> == false );
        // CHECK( t1.error != t2.error );
      }

      THEN( "error1.value == error2.value where value have the same integer and casting them" ) {
        CHECK( static_cast<unsigned int>( t1.error ) == static_cast<unsigned int>( t2.error ) );
      }
    }

    WHEN( "Error bit flags" ) {

      struct TestErrorCode1 : BaseErrorCode<> {
        Error<TestErrorCode1> one   = { 1 << 1, "one" };
        Error<TestErrorCode1> two   = { 1 << 2, "two" };
        Error<TestErrorCode1> alias = 1 << 1;
      };

      using TestError1 = Error<TestErrorCode1>;

      TestErrorCode1 t1;

      THEN( "error1 & error1 returns error1" ) {
        static_assert( std::is_same_v<decltype( t1.one & t1.one ), TestError1>, "operator should return same type" );
        CHECK( static_cast<TestError1::ValueType>( t1.one & t1.one ) == t1.one.value );
      }

      THEN( "error1 & error2 returns new error" ) {
        static_assert( std::is_same_v<decltype( t1.one & t1.two ), TestError1>, "operator should return same type" );
        CHECK( static_cast<TestError1::ValueType>( t1.one & t1.two ) == 0 );
        CHECK( static_cast<bool>( t1.one & t1.two ) == false );
      }

      THEN( "error1 | error2 returns new error" ) {
        static_assert( std::is_same_v<decltype( t1.one | t1.two ), TestError1>, "operator should return same type" );
        const auto n = t1.one | t1.two;
        CHECK( static_cast<bool>( t1.one & n ) == true );
      }

      THEN( "error1 | alias returns error1" ) {
        static_assert( std::is_same_v<decltype( t1.one | t1.alias ), TestError1>, "operator should return same type" );
        const auto n = t1.one | t1.alias;
        CHECK( static_cast<bool>( t1.one & n ) == true );
      }

      THEN( "any works" ) {
        static_assert(
          std::is_same_v<decltype( t1.any ), const BaseError<TestError1::ValueType>>, "should return base type"
        );
        const auto n = t1.one | t1.two;
        CHECK( static_cast<bool>( t1.any & n ) == true );
      }

      THEN( "unknown works" ) {
        static_assert(
          std::is_same_v<decltype( t1.unknown ), const BaseError<TestError1::ValueType>>, "should return base type"
        );
        const auto n = t1.one | t1.two;
        CHECK( static_cast<bool>( t1.unknown & n ) == false );
      }
    }
  }
}

SCENARIO( "Maybe", "[error]" ) {

  GIVEN( "Maybe" ) {

    WHEN( "Maybe" ) {

      struct TestErrorCode : BaseErrorCode<> {
        Error<TestErrorCode> one   = { 1 << 1, "one" };
        Error<TestErrorCode> two   = { 1 << 2, "two" };
        Error<TestErrorCode> alias = { 1 << 1, "alias" };
      };

      using TestError = Error<TestErrorCode>;

      TestErrorCode t;
      TestErrorCode t2;

      THEN( "maybe works" ) {
        maybe<int, TestError> m1( 2 );
        maybe<int, TestError> m2( t.two );

        CHECK( m1.has_value() == true );
        CHECK( m1.value() == 2 );
        CHECK( m2.has_value() == false );
        CHECK( m2.error() == t.two );
      }
    }
  }
}