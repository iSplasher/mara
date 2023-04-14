
#pragma once

#include <memory>
#include <utility>

#include "./external/peeters_tree.h"
#include "common.h"
#include "error.h"

NAMESPACE_BEGIN

// forward declarations
template<typename T>
class SubTree;

// Errors
// -----------------------------------------------------------------------------

struct TreeErrorCode : BaseErrorCode<> {
  inline static Error<TreeErrorCode> tree_already_destroyed   = { 1 << 1, "Tree already destroyed" };
  inline static Error<TreeErrorCode> only_single_root_allowed = { 1 << 2, "Only a single root is allowed" };
};

using TreeError = Error<TreeErrorCode>;

// -----------------------------------------------------------------------------

/**
 * @brief A tree data structure.
 *
 * @tparam T
 */
template<typename T>
class Tree : public SubTree<T> {

public:
  using Element             = typename SubTree<T>::Element;
  using ElementIterator     = typename SubTree<T>::ElementIterator;
  using ElementTreeIterator = typename SubTree<T>::ElementTreeIterator;

  Tree<T>( T&& root )
    : SubTree<T>() {
    this->_internal_tree = std::make_shared<tree<T>>();

    auto it              = _internal_tree->insert( _internal_tree->end(), root );
    this->init( _internal_tree, std::move( it ) );
  }

  ~Tree<T>() = default;

private:
  std::shared_ptr<tree<T>> _internal_tree = nullptr;
};

// -----------------------------------------------------------------------------

template<typename T>
struct BreathFirstIterator : public tree<T>::breadth_first_iterator {

  using iterator = typename tree<T>::breadth_first_iterator;

  BreathFirstIterator( const typename tree<T>::sibling_iterator& it )
    : iterator( it ) {}

  // make these private since they return sibling_iterators

private:
  typename tree<T>::sibling_iterator begin() const { return tree<T>::breadth_first_iterator::begin(); }

  typename tree<T>::sibling_iterator end() const { return tree<T>::breadth_first_iterator::end(); }
};

// -----------------------------------------------------------------------------

template<typename T>
class SubTree {

  using InternalTree    = tree<T>;
  using InternalTreePtr = std::shared_ptr<InternalTree>;

protected:
  using Element             = typename InternalTree::sibling_iterator;
  using ElementIterator     = typename InternalTree::sibling_iterator;
  using ElementTreeIterator = BreathFirstIterator<T>;

  // Regular constructors are protected to prevent direct instantiation. Instantiate Tree instead.

  SubTree<T>( std::weak_ptr<InternalTree> _internal_tree, Element&& root ) {
    init( _internal_tree, std::move( root ) );
  }

  SubTree<T>() { _root = Element(); };

public:
  SubTree<T>( SubTree<T>&& )            = default;

  SubTree<T>& operator=( SubTree<T>&& ) = default;

  virtual ~SubTree<T>()                 = default;

  /**
   * @brief Add a child to the tree, and return a sub-tree with child as root.
   *
   * @param child
   * @return SubTree new sub-tree with child as root.
   */
  maybe<SubTree<T>, TreeError> add_child( const T&& child ) {
    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }
    return SubTree<T>( internal_tree, internal_tree.lock()->append_child( this->_root, child ) );
  }

  /**
   * @brief Add a node at the same depth as the current root.
   *
   * @param child
   * @return SubTree reference to the same sub-tree.
   */
  maybe<SubTree<T>&, TreeError> append( T&& child ) {

    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }

    InternalTreePtr it = internal_tree.lock();

    // if ( this->_root == it->) {
    //   return TreeErrorCode::only_single_root_allowed;
    // }

    return *this;
  }

  /**
   * @brief Iterate the tree in breadth-first order. Root included.
   *
   * @return ElementTreeIterator
   */
  maybe<ElementTreeIterator, TreeError> iter() const {

    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }

    const auto s = internal_tree.lock();

    return ElementTreeIterator( this->_root );
  }

  /**
   * @brief Iterate the tree in breadth-first order. Root included.
   *
   * @return ElementTreeIterator
   */
  ElementTreeIterator unsafe_iter() const { return iter().value(); }

  /**
   * @brief Iterate the children at depth. Depth = 0 is direct children.
   *
   * @return ElementIterator
   */
  maybe<ElementIterator, TreeError> iter_depth( int depth = 0 ) const {
    if ( depth != 0 ) {
      NOT_IMPLEMENTED
    }

    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }

    const auto s = internal_tree.lock();

    auto child   = s->begin( _root );

    return child;
  }

  /**
   * @brief Iterate the children at depth. Depth = 0 is direct children.
   *
   * @return ElementIterator
   */
  ElementIterator unsafe_iter_depth( int depth = 0 ) const { return iter_depth( depth ).value(); }

  /**
   * @brief Iterate all the children in the tree in breadth-first order.
   *
   * @return ElementTreeIterator
   */
  maybe<ElementTreeIterator, TreeError> iter_children() const {

    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }

    auto it = ElementTreeIterator( _root );
    it++;

    return it;
  }

  /**
   * @brief Iterate all the children in the tree in breadth-first order.
   *
   * @return ElementTreeIterator
   */
  ElementTreeIterator unsafe_iter_children() const { return iter_children().value(); }

  /**
   * @brief Total size of the tree, inclusive root.
   *
   * @return size_t
   */
  maybe<size_t, TreeError> size() const {

    const auto s = children_size();

    return s.has_value() ? s.value() + 1 : s;
  }

  /**
   * @brief Total size of the tree, inclusive root.
   *
   * @return size_t
   */
  size_t unsafe_size() const { return size().value(); }

  /**
   * @brief Size at root depth. Root included.
   *
   * @return size_t
   */
  maybe<size_t, TreeError> siblings_size() const {
    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }
    const auto s = internal_tree.lock();

    return s->number_of_siblings( _root ) + 1;
  };

  /**
   * @brief Size at root depth. Root included.
   *
   * @return size_t
   */
  size_t unsafe_siblings_size() const { return siblings_size().value(); }

  /**
   * @brief Size at given depth. Depth = 0 is direct children.
   *
   * @return size_t
   */
  maybe<size_t, TreeError> depth_size( int depth = 0 ) const {
    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }
    if ( depth != 0 ) {
      NOT_IMPLEMENTED
    }
    const auto s = internal_tree.lock();
    if ( s->number_of_children( _root ) == 0 ) {
      return 0;
    }

    const auto child = s->begin( _root );

    return s->number_of_siblings( child );
  };

  /**
   * @brief Size at given depth. Depth = 0 is direct children.
   *
   * @return size_t
   */
  size_t unsafe_depth_size( int depth = 0 ) const { return depth_size( depth ).value(); };

  /**
   * @brief Size of how many children this root has.
   *
   * @return size_t
   */
  maybe<size_t, TreeError> children_size() const {
    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }
    return internal_tree.lock()->number_of_children( _root );
  };

  /**
   * @brief Size of how many children this root has.
   *
   * @return size_t
   */
  size_t unsafe_children_size() const { return children_size().value(); };

  /**
   * @brief Get the root value.
   *
   * @return T&
   */
  T& value() const { return *this->_root; }

  /**
   * @brief Get the root.
   *
   * @return Element&
   */
  const Element& root() const { return this->_root; }

  /**
   * @brief Get value position in the tree.
   *
   * @return T&
   */
  T& get( const Element& el ) const { return el.value(); }

  /**
   * @brief Check if element is valid in the tree.
   *
   * @return bool
   */
  bool is_valid( const typename InternalTree::iterator_base& el ) const {
    if ( internal_tree.expired() ) {
      return false;
    }

    auto s = internal_tree.lock();

    return s->is_valid( el );
  }

  /**
   * @brief Get sub-tree at position in the tree.
   *
   * @return SubTree<T>
   */
  SubTree<T> at( const Element& el ) const {
    auto s          = SubTree<T>();
    s.internal_tree = internal_tree;
    s._root         = el;

    return s;
  }

protected:
  void init( std::weak_ptr<InternalTree> _internal_tree, Element&& root ) {
    this->internal_tree = _internal_tree;
    this->_root         = root;
  }

  Element _root;

  std::weak_ptr<InternalTree> internal_tree;
};

// print the tree horizontally
template<typename T>
std::string& print_tree( const SubTree<T>& tree, std::string& out, std::string prefix, std::string children_prefix ) {

  out          += prefix + fmt::format( "{}\n", tree.value() );

  const auto s = tree.depth_size().value_or( 0 );
  if ( s ) {

    auto maybe_it = tree.iter_depth();

    if ( maybe_it.has_value() ) {
      auto   it = maybe_it.value();
      size_t i  = 0;

      while ( tree.is_valid( it ) ) {
        const bool right = i == s;

        const auto st    = tree.at( it );

        if ( right ) {
          print_tree( st, out, children_prefix + "└── ", children_prefix + "    " );
        } else {
          print_tree( st, out, children_prefix + "├── ", children_prefix + "│   " );
        }

        i++;
        it++;
      }
    }
  }
  return out;
}

template<typename T>
std::string print_tree( const SubTree<T>& tree ) {
  std::string s = "";

  print_tree( tree, s, "", "" );
  return s;
}

// template<typename T>
// std::ostream& operator<<( std::ostream& stream, const SubTree<T>& tree ) {
//   stream << error.message.size() ? fmt::format( "{}|{}", val, error.message ) : val;
//   return stream;
// };

NAMESPACE_END