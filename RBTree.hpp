#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "TreeIterator.hpp"
#include "TreeNode.hpp"

template<typename T, typename Compare, typename Allocator>
class RBTree
{
public:
	typedef T														value_type;
	typedef Allocator												allocator_type;
	typedef Compare													value_compare;
	typedef TreeNode<T>												node_type;
	typedef typename Allocator::template rebind<node_type>::other	node_alloc_type;

	value_compare		_val_comp;
	allocator_type		_alloc;
	node_alloc_type 	_node_alloc;
	node_type*			_root;
	node_type           _end;

	RBTree():
		_val_comp(),
		_alloc(),
		_node_alloc(),
		_root(node_type::nil()),
		_end(_root) {}

	explicit RBTree(const Compare& comp,
					const Allocator& alloc):
		_val_comp(comp),
		_alloc(alloc),
		_node_alloc(),
		_root(node_type::nil()),
		_end(_root) {}

	RBTree(const RBTree& other):
		_val_comp(other._val_comp),
		_alloc(other._alloc),
		_node_alloc(other._node_alloc),
		_root(_alloc_and_copy(&_end, other._root),
		_end(_root) {}

	void _update_root(node_type* root)
	{
		_root = root;
		_end.left = root;
		_end.right = root;
	}

	void _rotate_left(node_type* n)
	{
		node_type* pivot = n->right;
		pivot->parent = n->parent;
		if (n->parent != &_end)
		{
			if (n->parent->left == n)
				n->parent->left = pivot;
			else
				n->parent->right = pivot;
		}
		else
			_update_root(pivot);
		n->right = pivot->left;
		if (!pivot->left->is_nil())
			pivot->left->parent = n;
		n->parent = pivot;
		pivot->left = n;
	}

	void _rotate_right(node_type* n)
	{
		node_type* pivot = n->left;
		pivot->parent = n->parent;
		if (n->parent != &_end)
		{
			if (n->parent->left == n)
				n->parent->left = pivot;
			else
				n->parent->right = pivot;
		}
		else
			_update_root(pivot);
		n->left = pivot->right;
		if (!pivot->right->is_nil())
			pivot->right->parent = n;
		n->parent = pivot;
		pivot->right = n;
	}

	void _insert_case1(node_type* n)
	{
		if (n->parent == &_end)
		{
			_update_root(n);
			n->is_red = false;
		}
		else
			_insert_case2(n);
	}

	void _insert_case2(node_type* n)
	{
		if (n->parent->is_red)
			_insert_case3(n);
	}

	void _insert_case3(node_type* n)
	{
		node_type* u = n->uncle();
		node_type* g;
		if (!u->is_nil() && u->is_red)
		{
			n->parent->is_red = false;
			u->is_red = false;
			g = n->grandparent();
			g->is_red = true;
			_insert_case1(g);
		}
		else
			_insert_case4(n);
	}

	void _insert_case4(node_type* n)
	{
		node_type* g = n->grandparent();
		node_type* ptr = n;
		if (n == n->parent->right && n->parent == g->left)
		{
			_rotate_left(n->parent);
			ptr = n->left;
		}
		else if (n == n->parent->left && n->parent == g->right)
		{
			_rotate_right(n->parent);
			ptr = n->right;
		}
		_insert_case5(ptr);
	}

	void _insert_case5(node_type* n)
	{
		node_type* g = n->grandparent();
		n->parent->is_red = false;
		g->is_red = true;
		if (n == n->parent->left && n->parent == g->left)
			_rotate_right(g);
		else
			_rotate_left(g);
	}

	void _replace_node(node_type* n, node_type* child)
	{
		child->parent = n->parent;
		if (n->parent == &_end)
			_update_root(child);
		else
			n->child_pointer() = child;
	}

	void _node_swap(node_type* root, node_type* leaf)
	{
		if (root->parent != &_end)
			root->child_pointer() = leaf;
		else
			_update_root(leaf);
		if (root->right == leaf)
		{
			leaf->parent = root->parent;
			root->right = leaf->right;
			leaf->right = root;
		}
		else
		{
			leaf->child_pointer() = root;
			std::swap(root->parent, leaf->parent);
			std::swap(root->right, leaf->right);
		}
		std::swap(root->left, leaf->left);
		std::swap(root->is_red, leaf->is_red);
		if (!leaf->left->is_nil())
			leaf->left->parent = leaf;
		if (!leaf->right->is_nil())
			leaf->right->parent = leaf;
	}

	void _delete_one_child(node_type* n)
	{
		node_type* child = n->right->is_nil() ? n->left : n->right;

		_replace_node(n, child);
		if (!n->is_red)
		{
			if (child->is_red)
				child->is_red = false;
			else
				_delete_case1(child);
		}
		_destroy_and_dealloc_node(n);
	}

	void _delete_case1(node_type* n)
	{
		if (n->parent != &_end)
			_delete_case2(n);
	}

	void _delete_case2(node_type* n)
	{
		node_type* s = n->sibling();

		if (s->is_red) {
			n->parent->is_red = true;
			s->is_red = false;
			if (n == n->parent->left)
				_rotate_left(n->parent);
			else
				_rotate_right(n->parent);
		}
		_delete_case3(n);
	}

	void _delete_case3(node_type* n)
	{
		node_type *s = n->sibling();

		if ((!n->parent->is_red) &&
			(!s->is_red) &&
			(!s->left->is_red) &&
			(!s->right->is_red))
		{
			s->is_red = true;
			_delete_case1(n->parent);
		}
		else
			_delete_case4(n);
	}

	void _delete_case4(node_type* n)
	{
		node_type* s = n->sibling();

		if ((n->parent->is_red) &&
			(!s->is_red) &&
			(!s->left->is_red) &&
			(!s->right->is_red))
		{
			s->is_red = true;
			n->parent->is_red = false;
		}
		else
			_delete_case5(n);
	}

	void _delete_case5(node_type* n)
	{
		node_type *s = n->sibling();

		if (!s->is_red)
		{
			if ((n == n->parent->left) &&
				(!s->right->is_red) &&
				(s->left->is_red))
			{
				s->is_red = true;
				s->left->is_red = false;
				_rotate_right(s);
			}
			else if ((n == n->parent->right) &&
					 (!s->left->is_red) &&
					 (s->right->is_red))
			{
				s->is_red = true;
				s->right->is_red = true;
				_rotate_left(s);
			}
		}
		_delete_case6(n);
	}

	void _delete_case6(node_type* n)
	{
		node_type* s = n->sibling();

		s->is_red = n->parent->is_red;
		n->parent->is_red = false;

		if (n == n->parent->left) {
			s->right->is_red = false;
			_rotate_left(n->parent);
		} else {
			s->left->is_red = false;
			_rotate_right(n->parent);
		}
	}

	void _destroy_and_dealloc_node(node_type* n)
	{
		_alloc.destroy(n->value);
		_alloc.deallocate(n->value, 1u);
		_node_alloc.destroy(n);
		_node_alloc.deallocate(n, 1u);
	}

	void _destroy_and_dealloc(node_type* root)
	{
		if (root == node_type::nil())
			return;
		_destroy_and_dealloc(root->left);
		_destroy_and_dealloc(root->right);
		_destroy_and_dealloc_node(root);
	}

	node_type*	_alloc_and_init_node(node_type* parent, bool is_red, const value_type& value)
	{
		node_type* new_node = _node_alloc.allocate(1u);
		try
		{
			_node_alloc.construct(new_node, node_type(parent, is_red));
			try
			{
				new_node->value = _alloc.allocate(1u);
				try
				{ _alloc.construct(new_node->value, value); }
				catch (...)
				{ _alloc.deallocate(new_node->value, 1u); throw; }
			}
			catch (...)
			{ _node_alloc.destroy(new_node); throw; }
		}
		catch (...)
		{ _node_alloc.deallocate(new_node, 1u); throw; }
		return new_node;
	}

	node_type*	_alloc_and_copy(node_type* parent, node_type* root)
	{
		if (root == node_type::nil())
			return node_type::nil();
		node_type* new_node = _alloc_and_init_node(parent, root->is_red, *root->value);
		try
		{
			new_node->left  = _alloc_and_copy(new_node,root->left);
			try
			{ new_node->right = _alloc_and_copy(new_node, root->right); }
			catch (...)
			{ _destroy_and_dealloc(new_node->left); throw; }
		}
		catch (...)
		{ _destroy_and_dealloc(new_node); throw; }
		if (new_node->left != node_type::nil())
			new_node->left->parent = new_node;
		if (new_node->right != node_type::nil())
			new_node->right->parent = new_node;
		return new_node;
	}

	node_type*  _find(const value_type& key, node_type* root) const
	{
		node_type* ptr = root != &_end ? root : root->left;
		node_type* parent = node_type::nil();

		while (!ptr->is_nil())
		{
			parent = ptr;
			if (!_key_comp(ptr->value->first, key))
			{
				if (!_key_comp(key, ptr->value->first))
					return ptr;
				else
					ptr = ptr->left;
			}
			else
				ptr = ptr->right;
		}
		return parent;
	}

	node_type*  _find(node_type* hint, const value_type& key) const
	{
		node_type* ptr = hint != &_end ? hint : hint->left;
		node_type* parent = ptr;

		if (!ptr || ptr->is_nil())
			return _find(key, _root);
		if (_key_comp(ptr->value->first, key))
		{
			while (parent->parent != &_end && parent != parent->parent->left)
				parent = parent->parent;
			if (_key_comp(key, parent->value->first))
				return _find(key, ptr);
		}
		else
		{
			while (parent->parent != &_end && parent != parent->parent->right)
				parent = parent->parent;
			if (_key_comp(parent->value->first, key))
				return _find(key, ptr);
		}
		return _find(key, _root);
	}

	node_type*	_insert(const value_type& value)
	{

	}
};

#endif
