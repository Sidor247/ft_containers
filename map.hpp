#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <limits>
#include <iterator>
#include "utility.hpp"
#include "TreeNode.hpp"
#include "TreeIterator.hpp"

namespace ft
{
	template<
			class Key,
			class T,
			class Compare = std::less<Key>,
			class Allocator = std::allocator<ft::pair<const Key, T> >
	> class map
	{
        typedef TreeNode<ft::pair<const Key, T> >       node_type;
        
	public:
		typedef Key 								    key_type;
		typedef T									    mapped_type;
		typedef ft::pair<const Key, T> 				    value_type;
		typedef std::size_t 						    size_type;
		typedef std::ptrdiff_t 						    difference_type;
		typedef Compare 							    key_compare;
		typedef Allocator 							    allocator_type;
		typedef value_type& 						    reference;
		typedef const value_type& 					    const_reference;
		typedef typename Allocator::pointer 		    pointer;
		typedef typename Allocator::const_pointer	    const_pointer;
        typedef TreeIterator<node_type, false>			iterator;
        typedef	TreeIterator<node_type, true> 			const_iterator;
        typedef	ft::reverse_iterator<iterator> 			reverse_iterator;
        typedef	ft::reverse_iterator<const_iterator>    const_reverse_iterator;

		class value_compare : std::binary_function<value_type, value_type, bool>
		{
			friend class map;
		protected:
			Compare comp;

			value_compare( Compare c ): comp(c) {}

		public:
			bool operator()( const value_type& lhs, const value_type& rhs ) const
			{ return comp(lhs.first, rhs.first); }
		};

	private:
//        template<bool IsConst>
//        struct _ptr_unpacker : TreeIterator<node_type, IsConst>
//        {
//
//        };

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

        void node_swap(node_type* root, node_type* leaf)
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

            if  (!s->is_red)
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

		typedef typename Allocator::template rebind<node_type>::other	node_alloc_type;

		value_compare		_val_comp;
		allocator_type		_alloc;
		node_alloc_type 	_node_alloc;
		size_type 			_size;
        node_type           _end;
		node_type*			_root;
		node_type*			_first;
		node_type*			_last;

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

        node_type*  _find(const key_type& key, node_type* root) const
        {
            node_type* ptr = root;
            node_type* parent = nullptr;

            if (root->is_nil())
                return node_type::nil();
            while (!ptr->is_nil())
            {
                parent = ptr;
                if (!_val_comp.comp(ptr->value->first, key))
                {
                    if (!_val_comp.comp(key, ptr->value->first))
                        return ptr;
                    else
                        ptr = ptr->left;
                }
                else
                    ptr = ptr->right;
            }
            return parent;
        }

        node_type*  _find(node_type* hint, const key_type& key) const
        {
            node_type* ptr = hint;
            node_type* parent = ptr;

            if (_val_comp.comp(ptr->value->first, key))
            {
                while (parent->parent != &_end && parent != parent->parent->left)
                    parent = parent->parent;
                if (_val_comp.comp(key, parent->value->first))
                    return _find(key, ptr);
            }
            else
            {
                while (parent->parent != &_end && parent != parent->parent->right)
                    parent = parent->parent;
                if (_val_comp.comp(parent->value->first, key))
                    return _find(key, ptr);
            }
            return _find(key, _root);
        }

	public:
		map():
			_val_comp(Compare()),
			_alloc(),
            _node_alloc(),
			_size(),
            _end(),
			_root(node_type::nil()),
			_first(&_end),
			_last(&_end) {}

		explicit map( const Compare& comp,
					  const Allocator& alloc = Allocator() ):
					  _val_comp(comp),
					  _alloc(alloc),
                      _node_alloc(),
					  _size(),
                      _end(),
                      _root(node_type::nil()),
                      _first(&_end),
                      _last(&_end) {}

		template< class InputIt >
		map( InputIt first, InputIt last,
			 const Compare& comp = Compare(),
			 const Allocator& alloc = Allocator() ):
			 _val_comp(comp),
			 _alloc(alloc),
             _node_alloc(),
			 _size(),
             _end(),
             _root(node_type::nil()),
             _first(&_end),
             _last(&_end)
         { insert(first, last); }

		map( const map& other ):
			_val_comp(other._val_comp.comp),
			_alloc(other._alloc),
            _node_alloc(other._node_alloc),
			_size(other._size),
            _end(),
			_root(_alloc_and_copy(&_end, other._root)),
			_first(_root->advanced_left()),
			_last(_root->advanced_right())
		{
			_end.left = _root;
			_end.right = _root;
		}

	 	~map()
		 { _destroy_and_dealloc(_root); }

		map& operator=( const map& other )
		{
			if (this == &other)
				return *this;
			node_type* new_tree = _alloc_and_copy(&_end, other._root);
			_destroy_and_dealloc(_root);
			_val_comp = other._val_comp.comp;
			_alloc = other._alloc;
            _node_alloc = other._node_alloc;
			_size = other._size;
            _update_root(new_tree);
			_first = _root->advanced_left();
			_last = _root->advanced_right();
            return *this;
		}

		allocator_type get_allocator() const
		{ return _alloc; }

		T& at( const Key& key )
		{
            iterator it = find(key);
			if (it == end())
				throw std::out_of_range("out_of_range");
			return it->second;
		}

		const T& at( const Key& key ) const
		{
            const_iterator it = find(key);
			if (it == end())
				throw std::out_of_range("out_of_range");
			return it->second;
		}

		T& operator[]( const Key& key )
		{ return insert(value_type(key, T())).first->second; }

		iterator begin()
		{ return iterator(_first); }

        const_iterator begin() const
        { return const_iterator(_first); }

        iterator end()
        { return iterator(&_end); }

        const_iterator end() const
        { return const_iterator(&_end); }

        reverse_iterator rbegin()
        { return reverse_iterator(iterator(_last)); }

        const_reverse_iterator rbegin() const
        { return const_reverse_iterator(const_iterator(_last)); }

        reverse_iterator rend()
        { return reverse_iterator(iterator(&_end)); }

        const_reverse_iterator rend() const
        { return const_reverse_iterator(const_iterator(&_end)); }

		bool empty() const
		{ return !_size; }

		size_type size() const
		{ return _size; }

		size_type max_size() const
		{ return std::numeric_limits<difference_type>::max(); }

		void clear()
		{
			_destroy_and_dealloc(_root);
            _end.left = nullptr;
            _end.right = nullptr;
			_root = node_type::nil();
            _first = &_end;
            _last = &_end;
			_size = 0;
		}

		ft::pair<iterator, bool> insert(const value_type& value)
		{
            node_type* pos = _find(value.first, _root);
            if (!pos->is_nil() && !_val_comp(*pos->value, value) && !_val_comp(value, *pos->value))
                return ft::make_pair(iterator(pos), false);
			node_type* new_node = _alloc_and_init_node(pos->is_nil() ? &_end : pos, true, value);
            if (!pos->is_nil())
            {
                if (_val_comp(value, *pos->value))
                    pos->left = new_node;
                else
                    pos->right = new_node;
            }
            else
                _update_root(new_node);
			_insert_case1(new_node);
			_first = _root->advanced_left();
			_last = _root->advanced_right();
			++_size;
			return ft::make_pair(iterator(new_node), true);
		}

		iterator insert(iterator hint, const value_type& value)
		{
            node_type* pos = _find(hint._ptr, value.first);
            if (!pos->is_nil() && !_val_comp(*pos->value, value) && !_val_comp(value, *pos->value))
                return iterator(pos);
            node_type* new_node = _alloc_and_init_node(pos->is_nil() ? &_end : pos, true, value);
            if (!pos->is_nil())
            {
                if (_val_comp(value, *pos->value))
                    pos->left = new_node;
                else
                    pos->right = new_node;
            }
            else
                _update_root(new_node);
            _insert_case1(new_node);
            _first = _root->advanced_left();
            _last = _root->advanced_right();
            ++_size;
            return iterator(new_node);
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
            if (first == last)
                return;
            iterator hint = insert(*first++).first;
            for (InputIterator it = first; it != last; ++it)
                hint = insert(hint, *it);
		}

		void erase( iterator pos )
        {
            node_type* ptr = pos._ptr;
            if (!ptr->left->is_nil() && !ptr->right->is_nil())
                node_swap(ptr, ptr->right->advanced_left());
            _delete_one_child(ptr);
            _first = _root->advanced_left();
            _last = _root->advanced_right();
            --_size;
        }

		void erase( iterator first, iterator last )
        {
            node_type* ptr;

            for (iterator it = first; it != last; ++it)
            {
                ptr = it._ptr;
                if (!ptr->left->is_nil() && !ptr->right->is_nil())
                    node_swap(ptr, ptr->right->advanced_left());
                _delete_one_child(ptr);
                --_size;
            }
            _first = _root->advanced_left();
            _last = _root->advanced_right();
        }

		size_type erase( const Key& key )
        {
            iterator pos = find(key);
            if (pos == end())
                return 0;
            erase(pos);
            return 1;
        }

		void swap( map& other )
        {
			node_type* tmp = _root;
			_update_root(other._root);
			other._update_root(tmp);
            _root->parent = &_end;
            other._root->parent = &other._end;
            std::swap(_val_comp, other._val_comp);
            std::swap(_alloc, other._alloc);
            std::swap(_node_alloc, other._node_alloc);
            std::swap(_size, other._size);
            std::swap(_root, other._root);
            std::swap(_first, other._first);
            std::swap(_last, other._last);
        }

		size_type count( const Key& key ) const
		{ return find(key) == end() ? 0 : 1; }

		iterator find( const Key& key )
		{
            node_type* ptr = _find(key, _root);
            if (!ptr->is_nil() &&
				!_val_comp.comp(ptr->value->first, key) &&
				!_val_comp.comp(key, ptr->value->first))
                return iterator(ptr);
            else
			    return end();
		}

		const_iterator find( const Key& key ) const
		{
            node_type* ptr = _find(key, _root);
            if (!ptr->is_nil() &&
				!_val_comp.comp(ptr->value->first, key) &&
				!_val_comp.comp(key, ptr->value->first))
                return const_iterator(ptr);
            else
                return end();
		}

		ft::pair<iterator,iterator> equal_range( const Key& key )
		{ return ft::make_pair(lower_bound(key), upper_bound(key)); }

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
		{ return ft::make_pair(lower_bound(key), upper_bound(key)); }

		iterator lower_bound( const Key& key )
		{
			node_type* tmp = _root;
			node_type* prev = nullptr;
			while (tmp != node_type::nil())
			{
				if (key == tmp->value->first)
					return iterator(tmp);
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && _val_comp.comp(key, prev->value->first) ? iterator(prev) : end();
		}

		const_iterator lower_bound( const Key& key ) const
		{
			node_type* tmp = _root;
			node_type* prev = nullptr;
			while (tmp != node_type::nil())
			{
				if (key == tmp->value->first)
					return const_iterator(tmp);
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && _val_comp.comp(key, prev->value->first) ? const_iterator(prev) : end();
		}

		iterator upper_bound( const Key& key )
		{
            node_type* tmp = _root;
            node_type* prev = nullptr;
            while (tmp != node_type::nil())
            {
                if (key == tmp->value->first)
                    return iterator(tmp);
                if (_val_comp.comp(key, tmp->value->first))
                {
                    prev = tmp;
                    tmp = tmp->left;
                }
                else
                    tmp = tmp->right;
            }
            return prev && !_val_comp.comp(key, prev->value->first) ? iterator(prev) : end();
		}

		const_iterator upper_bound( const Key& key ) const
		{
			node_type* tmp = _root;
			node_type* prev = nullptr;
			while (tmp != node_type::nil())
			{
				if (key == tmp->value->first)
					return const_iterator(tmp);
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && !_val_comp.comp(key, prev->value->first) ? const_iterator(prev) : end();
		}

		value_compare value_comp() const
		{ return _val_comp; }

		key_compare key_comp() const
		{ return _val_comp.comp; }

		friend 	bool operator==(const map& lhs, const map& rhs )
		{ return lhs._size == rhs._size && ft::equal(lhs.begin(), lhs.end(), rhs.begin()); }

		friend 	bool operator!=(const map& lhs, const map& rhs )
		{ return !(lhs == rhs); }

		friend 	bool operator< (const map& lhs, const map& rhs )
		{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

		friend 	bool operator> (const map& lhs, const map& rhs )
		{ return rhs < lhs; }

		friend 	bool operator<=(const map& lhs, const map& rhs )
		{ return !(lhs > rhs); }

		friend 	bool operator>=(const map& lhs, const map& rhs )
		{ return !(lhs < rhs); }
	};
	
};

namespace std
{
	template< class Key, class T, class Compare, class Alloc >
	void swap( ft::map<Key,T,Compare,Alloc>& lhs,
			   ft::map<Key,T,Compare,Alloc>& rhs )
	{ lhs.swap(rhs); }
};

#endif
