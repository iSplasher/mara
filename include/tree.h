
#pragma once

#include <memory>

#include "./external/peeters_tree.h"
#include "common.h"

NAMESPACE_BEGIN

// forward declarations
template<typename T>
class SubTree;

// -----------------------------------------------------------------------------

/**
 * @brief A tree data structure.
 *
 * @tparam T
 */
template<typename T>
class Tree : public SubTree<T> {

  using Element         = typename SubTree<T>::Element;
  using ElementIterator = typename SubTree<T>::ElementIterator;

public:
  Tree<T>( T root )
    : SubTree<T>()
    , _internal_tree() {
    auto it = _internal_tree.insert( _internal_tree.end(), root );
    this->init( std::shared_ptr<tree<T>>( &_internal_tree ), std::move( it ) );
  }

  ~Tree<T>() = default;

private:
  tree<T> _internal_tree;
};

// -----------------------------------------------------------------------------

template<typename T>
class SubTree {

protected:
  using Element         = typename tree<T>::iterator;
  using ElementIterator = typename tree<T>::sibling_iterator;

  SubTree<T>( std::shared_ptr<tree<T>> _internal_tree, Element&& root ) { init( _internal_tree, std::move( root ) ); }

  SubTree<T>() { _root = Element(); };

public:
  SubTree<T>( SubTree<T>&& )            = default;
  ~SubTree<T>()                         = default;

  SubTree<T>& operator=( SubTree<T>&& ) = default;

  /**
   * @brief Add a child to the tree, and return a sub-tree with child as root.
   *
   * @param child
   * @return SubTree new sub-tree with child as root.
   */
  SubTree<T> add_child( T&& child ) {
    return SubTree<T>( internal_tree, internal_tree->append_child( this->_root, child ) );
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
  size_t size() { return children_size() + 1; }

  /**
   * @brief Size at the depth the root is at.
   *
   * @return size_t
   */
  size_t depth_size() { return internal_tree->number_of_siblings( _root ); };

  /**
   * @brief Size of how many children this root has.
   *
   * @return size_t
   */
  size_t children_size() { return internal_tree->number_of_children( _root ); };

protected:
  void init( std::shared_ptr<tree<T>> _internal_tree, Element&& root ) {
    this->internal_tree = _internal_tree;
    this->_root         = root;
  }

  Element _root;

private:
  std::shared_ptr<tree<T>> internal_tree = nullptr;
};

NAMESPACE_END