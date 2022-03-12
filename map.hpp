#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <limits>
#include <iterator>
#include "utility.hpp"

namespace ft
{
	template<
			class Key,
			class T,
			class Compare = std::less<Key>,
			class Allocator = std::allocator<ft::pair<const Key, T> >
	> class map
	{
	public:
		typedef Key 								key_type;
		typedef T									mapped_type;
		typedef ft::pair<const Key, T> 				value_type;
		typedef std::size_t 						size_type;
		typedef std::ptrdiff_t 						difference_type;
		typedef Compare 							key_compare;
		typedef Allocator 							allocator_type;
		typedef value_type& 						reference;
		typedef const value_type& 					const_reference;
		typedef typename Allocator::pointer 		pointer;
		typedef typename Allocator::const_pointer	const_pointer;

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
		struct _node
		{
			bool		is_red;
			_node*		parent;
			_node*		left;
			_node*		right;
			pointer 	value;

		private:
			_node():
				is_red(false),
				parent(nullptr),
				left(nullptr),
				right(nullptr),
				value(nullptr) {}

		public:
			static _node* nil()
			{
				static _node nil;
				return &nil;
			};

			bool is_nil()
			{ return this == nil(); }

			_node(_node* parent, bool is_red):
				is_red(is_red),
				parent(parent) {}

			_node(const _node& src):
				is_red(src.is_red),
				parent(src.parent),
				left(nil()),
				right(nil()),
				value(src.value) {}

            _node*  advanced_up()
            {
                _node* ptr = this;
                while (ptr->parent)
                    ptr = ptr->parent;
                return ptr;
            }

			_node*	advanced_left()
			{
				if (this == nil())
					return nil();
				_node* ptr = this;
				while (!ptr->left->is_nil())
					ptr = ptr->left;
				return ptr;
			}

			_node*	advanced_right()
			{
				if (this == nil())
					return nil();
				_node* ptr = this;
				while (!ptr->right->is_nil())
					ptr = ptr->right;
				return ptr;
			}

			_node*	grandparent() const
			{ return parent ? parent->parent : nullptr; }

			_node*	uncle() const
			{
				_node* g = grandparent();
				if (!g)
					return nil();
				if (parent == g->left)
					return g->right;
				else
					return g->left;
			}

			_node*	sibling()
			{
				if (this == parent->left)
					return parent->right;
				else
					return parent->left;
			}

            _node*& child_pointer()
            {
                if (this == parent->left)
                    return parent->left;
                else
                    return parent->right;
            }
		};

		void _rotate_left(_node* n)
		{
			_node* pivot = n->right;
			pivot->parent = n->parent;
			if (n->parent)
			{
				if (n->parent->left == n)
					n->parent->left = pivot;
				else
					n->parent->right = pivot;
			}
            else
                _root = pivot;
			n->right = pivot->left;
			if (!pivot->left->is_nil())
				pivot->left->parent = n;
			n->parent = pivot;
			pivot->left = n;
		}

		void _rotate_right(_node* n)
		{
			_node* pivot = n->left;
			pivot->parent = n->parent;
			if (n->parent)
			{
				if (n->parent->left == n)
					n->parent->left = pivot;
				else
					n->parent->right = pivot;
			}
            else
                _root = pivot;
			n->left = pivot->right;
			if (!pivot->right->is_nil())
				pivot->right->parent = n;
			n->parent = pivot;
			pivot->right = n;
		}

		void _insert_case1(_node* n)
		{
			if (!n->parent)
            {
                _root = n;
                n->is_red = false;
            }
			else
				_insert_case2(n);
		}

		void _insert_case2(_node* n)
		{
			if (n->parent->is_red)
				_insert_case3(n);
		}

		void _insert_case3(_node* n)
		{
			_node* u = n->uncle();
			_node* g;
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

		void _insert_case4(_node* n)
		{
			_node* g = n->grandparent();
			_node* ptr = n;
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

		void _insert_case5(_node* n)
		{
			_node* g = n->grandparent();
			n->parent->is_red = false;
			g->is_red = true;
			if (n == n->parent->left && n->parent == g->left)
				_rotate_right(g);
			else
				_rotate_left(g);
		}

		void _replace_node(_node* n, _node* child)
        {
			child->parent = n->parent;
            if (!n->parent)
                _root = child;
            else
                n->child_pointer() = child;
		}

        void _node_swap(_node* root, _node* leaf)
        {
            if (root->parent)
                root->child_pointer() = leaf;
            else
                _root = leaf;
            if (root->right == leaf)
            {
                leaf->parent = root->parent;
                leaf->right = root;
                root->right = _node::nil();
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

        void _delete_one_child(_node *n)
        {
            _node* child = n->right->is_nil() ? n->left : n->right;

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

        void _delete_case1(_node *n)
        {
            if (n->parent)
                _delete_case2(n);
        }

        void _delete_case2(_node *n)
        {
            struct _node *s = n->sibling();

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

        void _delete_case3(_node *n)
        {
            _node *s = n->sibling();

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

        void _delete_case4(_node *n)
        {
            _node *s = n->sibling();

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

        void _delete_case5(_node *n)
        {
            _node *s = n->sibling();

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

        void _delete_case6(_node *n)
        {
            _node *s = n->sibling();

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

		typedef typename Allocator::template rebind<_node>::other	_node_alloc_type;

		value_compare		_val_comp;
		Allocator			_alloc;
		_node_alloc_type 	_node_alloc;
		size_type 			_size;
		_node*				_root;
		_node*				_first;
		_node*				_last;

		template<bool IsConst>
		class _common_iterator : public std::iterator<
				std::bidirectional_iterator_tag,
				value_type,
				std::ptrdiff_t,
				typename ft::conditional<IsConst, const_pointer, pointer>::type,
				typename ft::conditional<IsConst, const_reference, reference>::type>
		{
            friend class map;

            _node* _ptr;
            _node* _prev;

		public:
            typedef typename ft::iterator_traits<_common_iterator>::iterator_category	iterator_category;
            typedef	typename ft::iterator_traits<_common_iterator>::value_type			value_type;
            typedef	typename ft::iterator_traits<_common_iterator>::difference_type		difference_type;
            typedef	typename ft::iterator_traits<_common_iterator>::pointer 			pointer;
            typedef	typename ft::iterator_traits<_common_iterator>::reference 			reference;

			_common_iterator(): _ptr(nullptr), _prev(nullptr) {}
			_common_iterator(_node* ptr, _node* prev): _ptr(ptr), _prev(prev) {}
			_common_iterator(const _common_iterator& src): _ptr(src._ptr), _prev(src._prev) {}

			_common_iterator&	operator=(const _common_iterator& src)
			{
				if (this == &src)
					return *this;
				_ptr = src._ptr;
				_prev = src._prev;
				return *this;
			}

			reference	operator*() const
			{ return *_ptr->value; }

			pointer		operator->() const
			{ return _ptr->value; }

			_common_iterator&	operator++()
			{
				if (_ptr)
				{
					_prev = _ptr;
					if (_ptr->right != _node::nil())
						_ptr = _ptr->right->advanced_left();
					else {
						_node *tmp = _ptr->parent;
						while (tmp && _ptr == tmp->right) {
							_ptr = tmp;
							tmp = tmp->parent;
						}
						_ptr = tmp;
					}
				}
				else
					_ptr = _prev;
				return *this;
			}

			_common_iterator operator++(int)
			{
				_common_iterator copy(*this);
				operator++();
				return copy;
			}

			_common_iterator&	operator--()
			{
				if (_ptr)
				{
					_prev = _ptr;
					if (_ptr->left != _node::nil())
						_ptr = _ptr->left->advanced_right();
					else {
						_node *tmp = _ptr->parent;
						while (tmp && _ptr == tmp->left) {
							_ptr = tmp;
							tmp = tmp->parent;
						}
						_ptr = tmp;
					}
				}
				else
					_ptr = _prev;
				return *this;
			}

			_common_iterator operator--(int)
			{
				_common_iterator copy(*this);
				operator--();
				return copy;
			}

			operator _common_iterator<true>()
			{ return _common_iterator<true>(_ptr, _prev); }

			friend	bool operator==(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return lhs._ptr == rhs._ptr; }

			friend	bool operator!=(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return lhs._ptr != rhs._ptr; }
		};

		void _destroy_and_dealloc_node(_node* n)
		{
			_alloc.destroy(n->value);
			_alloc.deallocate(n->value, 1u);
			_node_alloc.destroy(n);
			_node_alloc.deallocate(n, 1u);
		}

		void _destroy_and_dealloc(_node* root)
		{
			if (root == _node::nil())
				return;
			_destroy_and_dealloc(root->left);
			_destroy_and_dealloc(root->right);
			_destroy_and_dealloc_node(root);
		}

		_node*	_alloc_and_init_node(_node* parent, bool is_red, const value_type& value)
		{
			_node* newnode = _node_alloc.allocate(1u);
			try
			{
				_node_alloc.construct(newnode, _node(parent, is_red));
				try
				{
					newnode->value = _alloc.allocate(1u);
					try
					{ _alloc.construct(newnode->value, value); }
					catch (...)
					{ _alloc.deallocate(newnode->value, 1u); throw; }
				}
				catch (...)
				{ _node_alloc.destroy(newnode); throw; }
			}
			catch (...)
			{ _node_alloc.deallocate(newnode, 1u); throw; }
			return newnode;
		}

		_node*	_alloc_and_copy(_node* parent, _node* root)
		{
			if (root == _node::nil())
				return _node::nil();
			_node* newnode = _alloc_and_init_node(parent, root->is_red, *root->value);
			try
			{
				newnode->left  = _alloc_and_copy(newnode,root->left);
				try
				{ newnode->right = _alloc_and_copy(newnode, root->right); }
				catch (...)
				{ _destroy_and_dealloc(newnode->left); throw; }
			}
			catch (...)
			{ _destroy_and_dealloc(newnode); throw; }
			if (newnode->left != _node::nil())
				newnode->left->parent = newnode;
			if (newnode->right != _node::nil())
				newnode->right->parent = newnode;
			return newnode;
		}

		template<class InputIt>
		_node*	_alloc_and_init(InputIt, InputIt)
		{

		}

        _node*  _find(const key_type& key, _node* root)
        {
            _node* ptr = root;
            _node* parent = nullptr;

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

        _node*  _find(_node* hint, const key_type& key)
        {
            _node* ptr = hint;
            _node* parent = nullptr;

            while (ptr->parent)
            {
                parent = ptr->parent;
                if ((_val_comp.comp(parent->value->first, key) && ptr == parent->right)
                ||  (_val_comp.comp(key, parent->value->first) && ptr == parent->left))
                { break; }
                ptr = parent;
            }
            return _find(key, ptr);
        }

	public:
		typedef _common_iterator<false>					iterator;
		typedef	_common_iterator<true> 					const_iterator;
		typedef	ft::reverse_iterator<iterator> 			reverse_iterator;
		typedef	ft::reverse_iterator<const_iterator> 	const_reverse_iterator;

		map():
			_val_comp(Compare()),
			_alloc(Allocator()),
			_node_alloc(_node_alloc_type()),
			_size(size_type()),
			_root(_node::nil()),
			_first(_node::nil()),
			_last(_node::nil()) {}

		explicit map( const Compare& comp,
					  const Allocator& alloc = Allocator() ):
					  _val_comp(comp),
					  _alloc(alloc),
					  _node_alloc(alloc),
					  _size(size_type()),
					  _root(_node::nil()),
					  _first(_node::nil()),
					  _last(_node::nil()) {}

		template< class InputIt >
		map( InputIt first, InputIt last,
			 const Compare& comp = Compare(),
			 const Allocator& alloc = Allocator() ):
			 _val_comp(value_compare(comp)),
			 _alloc(alloc),
			 _node_alloc(alloc),
			 _size(size_type()),
			 _root(_alloc_and_init(first, last)),
			 _first(_root->advanced_left()),
			 _last(_root->advanced_right()) {}

		map( const map& other ):
			_val_comp(other._val_comp.comp),
			_alloc(other._alloc),
			_node_alloc(other._node_alloc),
			_size(other._size),
			_root(_alloc_and_copy(nullptr, other._root)),
			_first(_root->advanced_left()),
			_last(_root->advanced_right()) {}

	 	~map()
		 { _destroy_and_dealloc(_root); }

		map& operator=( const map& other )
		{
			if (this == &other)
				return *this;
			_node* newtree = _alloc_and_copy(other._root);
			_destroy_and_dealloc(_root);
			_val_comp = other._val_comp.comp;
			_alloc = other._alloc;
			_node_alloc = other._node_alloc;
			_size = other._size;
			_root = newtree;
			_first = _root->advanced_left();
			_last = _root->advanced_right();
		}

		allocator_type get_allocator() const
		{ return _alloc; }

		T& at( const Key& key )
		{
			const_iterator it = find(key);
			if (it == end())
				throw std::out_of_range("out_of_range");
			return it->second;
		}

		const T& at( const Key& key ) const
		{
			iterator it = find(key);
			if (it == end())
				throw std::out_of_range("out_of_range");
			return it->second;
		}

		T& operator[]( const Key& key )
		{ return insert(value_type(key, T())).first->second; }

		iterator begin()
		{ return iterator(!_first->is_nil() ?  _first : nullptr, nullptr); }

        const_iterator begin() const
        { return const_iterator(!_first->is_nil() ?  _first : nullptr, nullptr); }

        iterator end()
        { return iterator(nullptr, !_last->is_nil() ?  _last : nullptr); }

        const_iterator end() const
        { return const_iterator(nullptr, !_last->is_nil() ?  _last : nullptr); }

        reverse_iterator rbegin()
        { return reverse_iterator(iterator(!_last->is_nil() ? _last : nullptr, nullptr)); }

        const_reverse_iterator rbegin() const
        { return const_reverse_iterator(const_iterator(!_last->is_nil() ?  _last : nullptr, nullptr)); }

        reverse_iterator rend()
        { return reverse_iterator(iterator(nullptr, !_first->is_nil() ?  _first : nullptr)); }

        const_reverse_iterator rend() const
        { return const_reverse_iterator(const_iterator(nullptr, !_first->is_nil() ?  _first : nullptr)); }

		bool empty() const
		{ return !_size; }

		size_type size() const
		{ return _size; }

		size_type max_size() const
		{ return std::numeric_limits<difference_type>::max(); }

		void clear()
		{
			_destroy_and_dealloc(_root);
			_root = _node::nil();
			_size = 0;
		}

		ft::pair<iterator, bool> insert(const value_type& value)
		{
            _node* pos = _find(value.first, _root);
            if (pos && !_val_comp(*pos->value, value) && !_val_comp(value, *pos->value))
                return ft::make_pair(iterator(pos, nullptr), false);
			_node* new_node = _alloc_and_init_node(pos, true, value);
            if (pos)
            {
                if (_val_comp(value, *pos->value))
                    pos->left = new_node;
                else
                    pos->right = new_node;
            }
            else
                _root = new_node;
			_insert_case1(new_node);
			_first = _root->advanced_left();
			_last = _root->advanced_right();
			++_size;
			return ft::make_pair(iterator(new_node, nullptr), true);
		}

		iterator insert(iterator hint, const value_type& value)
		{
            _node* pos = _find(hint._ptr, value.first);
            if (pos && !_val_comp(*pos->value, value) && !_val_comp(value, *pos->value))
                return iterator(pos, nullptr);
            _node* new_node = _alloc_and_init_node(pos, true, value);
            if (pos)
            {
                if (_val_comp(value, *pos->value))
                    pos->left = new_node;
                else
                    pos->right = new_node;
            }
            else
                _root = new_node;
            _insert_case1(new_node);
            _first = _root->advanced_left();
            _last = _root->advanced_right();
            ++_size;
            return iterator(new_node, nullptr);
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
            if (first == last)
                return;
            iterator hint = insert(*++first).first;
            for (InputIterator it = first; it != last; ++it)
                hint = insert(hint, *it);
		}

		void erase( iterator pos )
        {
            _node* ptr = pos._ptr;
            if (!ptr->left->is_nil() && !ptr->right->is_nil())
                _node_swap(ptr, ptr->right->advanced_left());
            _delete_one_child(ptr);
            _first = _root->advanced_left();
            _last = _root->advanced_right();
            --_size;
        }

		void erase( iterator first, iterator last )
        {
            _node* ptr;

            for (iterator it = first; it != last; ++it)
            {
                ptr = it._ptr;
                if (!ptr->left->is_nil() && !ptr->right->is_nil())
                    _node_swap(ptr, ptr->right->advanced_left());
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
            _node* ptr = _find(key, _root);
            if (ptr && !_val_comp.comp(ptr->value->first, key)
                    && !_val_comp.comp(key, ptr->value->first))
                return iterator(ptr, nullptr);
            else
			    return end();
		}

		const_iterator find( const Key& key ) const
		{
            _node* ptr = _find(key);
            if (ptr && !_val_comp.comp(ptr->value->first, key)
                && !_val_comp.comp(key, ptr->value->first))
                return const_iterator(ptr, nullptr);
            else
                return end();
		}

		ft::pair<iterator,iterator> equal_range( const Key& key )
		{ return ft::make_pair(lower_bound(key), upper_bound(key)); }

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
		{ return ft::make_pair(lower_bound(key), upper_bound(key)); }

		iterator lower_bound( const Key& key )
		{
			_node* tmp = _root;
			_node* prev = nullptr;
			while (tmp != _node::nil())
			{
				if (key == tmp->value->first)
					return iterator(tmp, nullptr);
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && _val_comp.comp(key, prev->value->first) ? iterator(prev, nullptr) : end();
		}

		const_iterator lower_bound( const Key& key ) const
		{
			_node* tmp = _root;
			_node* prev = nullptr;
			while (tmp != _node::nil())
			{
				if (key == tmp->value->first)
					return const_iterator(tmp, nullptr);
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && _val_comp.comp(key, prev->value->first) ? const_iterator(prev, nullptr) : end();
		}

		iterator upper_bound( const Key& key )
		{
			_node* tmp = _root;
			_node* prev = nullptr;
			while (tmp != _node::nil())
			{
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && _val_comp.comp(key, prev->value->first) ? iterator(prev, nullptr) : end();
		}

		const_iterator upper_bound( const Key& key ) const
		{
			_node* tmp = _root;
			_node* prev = nullptr;
			while (tmp != _node::nil())
			{
				if (key == tmp->value->first)
					return const_iterator(tmp, nullptr);
				if (_val_comp.comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
				else
					tmp = tmp->right;
			}
			return prev && !_val_comp.comp(key, prev->value->first) ? const_iterator(prev, nullptr) : end();
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
		{ return !(rhs > lhs); }

		friend 	bool operator>=(const map& lhs, const map& rhs )
		{ return !(rhs < lhs); }
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
