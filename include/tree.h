
#pragma once

#include <memory>

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
  inline static Error<TreeErrorCode> tree_already_destroyed = { 1 << 1, "Tree already destroyed" };
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
  using Element         = typename SubTree<T>::Element;
  using ElementIterator = typename SubTree<T>::ElementIterator;

  Tree<T>( T root )
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
class SubTree {

protected:
  using Element         = typename tree<T>::iterator;
  using ElementIterator = typename tree<T>::sibling_iterator;

  // Regular constructors are protected to prevent direct instantiation. Instantiate Tree instead.

  SubTree<T>( std::weak_ptr<tree<T>> _internal_tree, Element&& root ) { init( _internal_tree, std::move( root ) ); }

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
  SubTree<T>& append( T&& child ) {
    NOT_IMPLEMENTED
    return *this;
  }

  /**
   * @brief Iterator the children
   *
   * @return ElementIterator
   */
  ElementIterator iter_children() { return this->_root.begin(); }

  /**
   * @brief Total size of the tree, inclusive root.
   *
   * @return size_t
   */
  maybe<size_t, TreeError> size() {

    const auto s = children_size();

    return s.has_value() ? s.value() + 1 : s;
  }

  /**
   * @brief Size at the depth the root is at (root inclusive)
   *
   * @return size_t
   */
  maybe<size_t, TreeError> depth_size() {
    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }
    return internal_tree.lock()->number_of_siblings( _root ) + 1;
  };

  /**
   * @brief Size of how many children this root has.
   *
   * @return size_t
   */
  maybe<size_t, TreeError> children_size() {
    if ( internal_tree.expired() ) {
      return TreeErrorCode::tree_already_destroyed;
    }
    return internal_tree.lock()->number_of_children( _root );
  };

protected:
  void init( std::weak_ptr<tree<T>> _internal_tree, Element&& root ) {
    this->internal_tree = _internal_tree;
    this->_root         = root;
  }

  Element _root;

  std::weak_ptr<tree<T>> internal_tree;
};

NAMESPACE_END