#include <catch2/catch_test_macros.hpp>
#include <forward_list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../include/tree.h"

using namespace rara;

SCENARIO( "Tree is well defined", "[tree]" ) {

  GIVEN( "A a tree to print" ) {
    int root = 0;

    Tree<int> tree( std::move( root ) );

    WHEN( "We add children" ) {
      for ( int i = 1; i < 10; ++i ) {
        auto c = i;
        tree.add_child( std::move( c ) );
      }
      THEN( "We get the correct number of children" ) {
        CHECK( tree.unsafe_children_size() == 9 );
      }
      // THEN( "We get the correct children" ) {
      //   CHECK( tree.children().front() == 1 );
      //   CHECK( tree.children().back() == 9 );
      // }
    }
  }

  GIVEN( "A tree with a single root" ) {
    int root = 0;

    Tree<int> tree( std::move( root ) );

    WHEN( "We add a single child" ) {

      auto it1 = tree.add_child( 1 );

      THEN( "The tree has no siblings because it's root" ) {
        CHECK( tree.unsafe_siblings_size() == 1 );
      }

      THEN( "The tree has two elements (with root)" ) {
        CHECK( tree.unsafe_size() == 2 );
      }

      THEN( "The tree has one child" ) {
        CHECK( tree.unsafe_children_size() == 1 );
      }

      THEN( "We can iterate the tree from root" ) {

        auto it   = tree.unsafe_iter();

        auto vals = std::forward_list( { 0, 1 } );

        int i     = 0;
        while ( tree.is_valid( it ) ) {
          i++;
          const auto v = *it;
          CHECK( vals.front() == v );
          vals.pop_front();
          it++;
        }

        CHECK( i == tree.unsafe_size() );
      }

      THEN( "We can iterate the children tree root inclusive" ) {

        auto it = tree.unsafe_iter();

        std::map<int, bool> vals;

        for ( auto i = 0; i != tree.unsafe_size(); ++i ) {
          vals[ i ] = false;
        }

        while ( tree.is_valid( it ) ) {
          const auto v = *it;
          vals[ v ]    = true;
          it++;
        }

        for ( auto i = 0; i != tree.unsafe_size(); ++i ) {
          CHECK( vals[ i ] == true );
        }
      }

      THEN( "We can iterate the children tree root exclusive" ) {
        auto it = tree.unsafe_iter_children();

        std::map<int, bool> vals;

        for ( auto i = 1; i != tree.unsafe_size(); ++i ) {
          vals[ i ] = false;
        }

        while ( tree.is_valid( it ) ) {
          const auto v = *it;
          vals[ v ]    = true;
          it++;
        }

        for ( auto i = 1; i != tree.unsafe_size(); ++i ) {
          const auto v = vals[ i ];
          CHECK( v == true );
        }
      }
    }

    THEN( "No children" ) {
      CHECK( tree.unsafe_children_size() == 0 );
    }

    WHEN( "We add many children" ) {

      for ( auto i = 1; i != 11; ++i ) {
        auto c = i;
        tree.add_child( std::move( c ) );
      }

      THEN( "We get the correct number of children" ) {
        CHECK( tree.unsafe_children_size() == 10 );
      }

      THEN( "We get the correct number of direct children" ) {
        CHECK( tree.unsafe_depth_size() == 10 );
      }

      THEN( "We can iterate breadth_first children tree root exclusive" ) {
        auto it   = tree.unsafe_iter_children();

        auto vals = std::forward_list( { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } );

        int i     = 0;
        while ( tree.is_valid( it ) ) {
          i++;
          const auto v = *it;
          CHECK( vals.front() == v );
          vals.pop_front();
          it++;
        }

        CHECK( i == ( tree.unsafe_size() - 1 ) );
      }

      THEN( "We can iterate breadth_first children tree root inclusive" ) {
        auto it   = tree.unsafe_iter();

        auto vals = std::forward_list( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } );

        int i     = 0;
        while ( tree.is_valid( it ) ) {
          i++;
          const auto v = *it;
          CHECK( vals.front() == v );
          vals.pop_front();
          it++;
        }

        CHECK( i == tree.unsafe_size() );
      }

      THEN( "We can print the tree" ) {

        auto str = print_tree( tree );

        CHECK( str == " " );
        CHECK( str.empty() == false );
      }
    }

    WHEN( "We construct a tree of children" ) {

      const int depth    = 5;
      const int children = 3;

      // construct a tree of depth 5 with 3 children using tree.add_child()
        auto root_tree = tree.at( tree.root() );
      for ( auto i = 0; i != depth; ++i ) {
        auto it        = root_tree.unsafe_iter_depth();
        for ( auto j = 0; j != children; ++j ) {
          auto c        = i * children + j;
          auto new_tree = root_tree.add_child( std::move( c ) );
          ++it;
          if ( tree.is_valid( it ) ) {
            root_tree = root_tree.at( it );
          }
          if ( j == children - 1 ) {
            root_tree = std::move( new_tree.value() );
          }
        }
      }
      THEN( "We get the correct number of children" ) {
        CHECK( tree.unsafe_children_size() == ( depth * children ) );
      }
      THEN( "We get the correct number of direct children" ) {
        CHECK( tree.unsafe_depth_size() == ( depth * children ) );
      }

      
      THEN( "We can iterate breadth_first children tree root exclusive" ) {
        auto it   = tree.unsafe_iter_children();

        auto vals = std::forward_list( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 } );
        int  i    = 0;
        while ( tree.is_valid( it ) ) {
          i++;
          const auto v = *it;
          CHECK( vals.front() == v );
          vals.pop_front();
          it++;
        }

        CHECK( i == ( tree.unsafe_size() - 1 ) );
      }

      THEN( "We can iterate breadth_first children tree root inclusive" ) {
        auto it   = tree.unsafe_iter();

        auto vals = std::forward_list( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 } );
        int  i    = 0;
        while ( tree.is_valid( it ) ) {
          i++;
          const auto v = *it;
          CHECK( vals.front() == v );
          vals.pop_front();
          it++;
        }

        CHECK( i == tree.unsafe_size() );
      }

      THEN( "We can print the tree" ) {
        auto str = print_tree( tree );
        CHECK( str == " " );
        CHECK( str.empty() == false );
      }
    }
  }
}