#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "TreeIterator.hpp"
#include "TreeNode.hpp"

template<typename T, typename Compare, typename Allocator>
class RBTree
{
	typedef T														value_type;
	typedef Allocator												allocator_type;
	typedef Compare													value_compare;
	typedef TreeNode<T>												node_type;
	typedef typename Allocator::template rebind<node_type>::other	node_alloc_type;

	value_compare		_comp;
	allocator_type		_alloc;
	node_alloc_type 	_node_alloc;
	node_type           _end;
	node_type*			_root;


};

#endif
