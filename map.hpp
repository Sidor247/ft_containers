#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <limits>
#include <iterator>
#include "pair.hpp"
#include "reverse_iterator.hpp"

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

			_node(_node* parent):
				is_red(true),
				parent(parent) {}

			_node(const _node& src):
				is_red(src.is_red),
				parent(src.parent),
				left(nil()),
				right(nil()),
				value(src.value) {}

			bool operator()()
			{ return this != nil(); }

			_node*	advanced_left()
			{
				if (this == nil())
					return nil();
				_node* ptr = this;
				while (ptr->left != nil())
					ptr = ptr->left;
				return ptr;
			}

			_node*	advanced_right()
			{
				if (this == nil())
					return nil();
				_node* ptr = this;
				while (ptr->right != nil())
					ptr = ptr->right;
				return ptr;
			}

			_node*	grandparent() const
			{ return parent ? parent->parent : nullptr; }

			_node*	uncle() const
			{
				_node* g = grandparent();
				if (!g)
					return _node::nil();
				if (parent == g->left)
					return g->left;
				else
					return g->right;
			}

			void rotate_left()
			{
				_node* pivot = right;
				pivot->parent = parent;
				if (parent)
				{
					if (parent->left == this)
						parent->left = pivot;
					else
						parent->right = pivot;
				}
				right = pivot->left;
				if (pivot->left != nil())
					pivot->left->parent = this;
				parent = pivot;
				pivot->left = this;
			}

			void rotate_right()
			{
				_node* pivot = left;
				pivot->parent = parent;
				if (parent)
				{
					if (parent->left == this)
						parent->left = pivot;
					else
						parent->right = pivot;
				}
				left = pivot->right;
				if (pivot->right != nil())
					pivot->right->parent = this;
				parent = pivot;
				pivot->right = this;
			}

			void insert_case1()
			{
				if (!parent)
					is_red = false;
				else
					insert_case2();
			}

			void insert_case2()
			{
				if (parent->is_red)
					insert_case3();
			}

			void insert_case3()
			{
				_node* u = uncle();
				_node* g;
				if (u != nil() && u->is_red)
				{
					parent->is_red = false;
					u->is_red = false;
					g = grandparent();
					g->is_red = true;
					g->insert_case1();
				}
				else
					insert_case4();
			}

			void insert_case4()
			{
				_node* g = grandparent();
				if (this == parent->right && parent == g->left)
				{
					parent->rotate_left();
					left->insert_case5();
				}
				else if (this == parent->left && parent == g->left)
				{
					parent->rotate_right();
					right->insert_case5();
				}
			}

			void insert_case5()
			{
				_node* g = grandparent();
				parent->is_red = false;
				g->is_red = true;
				if (this == parent->left && parent == g->left)
					g->rotate_right();
				else
					g->rotate_left();
			}
		};

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
				typename ft::conditional<IsConst, const_pointer, pointer>::type,
				typename ft::conditional<IsConst, const_reference, reference>::type>
		{
			_node* _ptr;
			_node* _prev;

        public:
			_common_iterator(): _ptr(nullptr), _prev(nullptr) {}
			_common_iterator(_node* ptr): _ptr(ptr), _prev(nullptr) {}
			_common_iterator(const _common_iterator& src): _ptr(src._ptr), _prev(src._prev) {}

			reference	operator*() const
			{ return *_ptr->value; }

			pointer		operator->() const
			{ return _ptr->value; }

			_common_iterator&	operator++()
			{
				if (_ptr->right != _prev && _ptr->right != _node::nil())
					_ptr = _ptr->right->advanced_left();
				else
				{
                    _prev = _ptr;
					_ptr = _ptr->parent;
				}
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
                if (_ptr->left != _prev && _ptr->left != _node::nil())
                    _ptr = _ptr->left->advanced_right();
                else
                {
                    _prev = _ptr;
                    _ptr = _ptr->parent;
                }
                return *this;
            }

            _common_iterator operator--(int)
            {
                _common_iterator copy(*this);
                operator--();
                return copy;
            }

    friend	bool operator==(const _common_iterator& lhs, const _common_iterator& rhs)
            { return lhs._prev == rhs._prev; }

    friend	bool operator!=(const _common_iterator& lhs, const _common_iterator& rhs)
            { return lhs._prev != rhs._prev; }
		};

		void _destroy_and_dealloc(_node* root)
		{
			if (root == _node::nil())
				return;
			_destroy_and_dealloc(root->left);
			_destroy_and_dealloc(root->right);
			_alloc.destroy(root->value);
			_alloc.deallocate(root->value, 1u);
			_node_alloc.destroy(root);
			_node_alloc.deallocate(root, 1u);
		}

		_node*	_alloc_and_init_node(_node* parent, const value_type& src)
		{
			_node* newnode = _node_alloc.allocate(1u);
			try
			{
				_node_alloc.construct(newnode, _node(parent));
				try
				{
					newnode->value = _alloc.allocate(1u);
					try
					{ _alloc.construct(newnode->value, src); }
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

		_node*	_alloc_and_copy(_node* root)
		{
			if (root == _node::nil())
				return _node::nil();
			_node* newnode = _alloc_and_init_node(nullptr, *root->value);
			try
			{
				newnode->left  = _alloc_and_copy(root->left);
				try
				{ newnode->right = _alloc_and_copy(root->right); }
				catch (...)
				{ _destroy_and_dealloc(newnode->left); throw; }
			}
			catch (...)
			{ _destroy_and_dealloc(newnode); }
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

	public:
		typedef _common_iterator<false>					iterator;
		typedef	_common_iterator<false> 				const_iterator;
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
					  _node_alloc(_node_alloc_type(alloc)),
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
			_val_comp(value_compare(other._val_comp.comp)),
			_alloc(other._alloc),
			_node_alloc(other._node_alloc),
			_size(other._size),
			_root(_alloc_and_copy(other._root)),
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

		iterator begin()
		{ return iterator(_first); }

        const_iterator begin() const
        { return const_iterator(_first); }

        iterator end()
        { return ++iterator(_last); }

        const_iterator end() const
        { return ++const_iterator(_last); }

        reverse_iterator rbegin()
        { return reverse_iterator(_last); }

        const_reverse_iterator rbegin() const
        { return const_reverse_iterator(_last); }

        reverse_iterator rend()
        { return ++reverse_iterator(_first); }

        const_reverse_iterator rend() const
        { return ++const_reverse_iterator(_last); }

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

		void insert(const value_type& val)
		{
			_node* tmp = _root;
			_node* parent = nullptr;
			_node** child;
            while (tmp != _node::nil())
            {
                parent = tmp;
                if (_val_comp(val, *tmp->value))
                    child = &tmp->left;
                else
                    child = &tmp->right;
                tmp = *child;
            }
			tmp = _alloc_and_init_node(parent, val);
			tmp->insert_case1();
			if (_root == _node::nil())
                _root = tmp;
            else
                *child = tmp;
			_first = _root->advanced_left();
			_last = _root->advanced_right();
			++_size;
		}

//		iterator insert(iterator position, const value_type& val)
//		{
//
//		}
//
//		template <class InputIterator>
//		void insert (InputIterator first, InputIterator last)
//		{}

		iterator find( const Key& key );

		const_iterator find( const Key& key ) const;

		value_compare value_comp() const
		{ return _val_comp; }

		key_compare key_comp() const
		{ return _val_comp.comp; }

	};

}

#endif
