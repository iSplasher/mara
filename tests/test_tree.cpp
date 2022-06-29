#include <catch2/catch_test_macros.hpp>

#include "../include/tree.h"

using namespace rara;

SCENARIO( "Tree is well formed", "[tree]" ) {

  GIVEN( "An empty tree" ) {
    constexpr int root = 0;

    Tree<int> tree( root );

    WHEN( "We add a single child" ) {

      auto it1 = tree.add_child( 1 );

      THEN( "The tree has no siblings because it's root" ) {
        REQUIRE( tree.depth_size() == 1 );
      }

      THEN( "The tree has two elements (with root)" ) {
        REQUIRE( tree.size() == 2 );
      }

      THEN( "The tree has one child" ) {
        REQUIRE( tree.children_size() == 1 );
      }
    }
  }
}