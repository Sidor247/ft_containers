#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <limits>
#include <iterator>
#include "utility.hpp"
#include "RBTree.hpp"
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
		typedef TreeNode<ft::pair<const Key, T> >       	node_type;

	public:
		typedef Key 								    	key_type;
		typedef T									    	mapped_type;
		typedef ft::pair<const Key, T> 				    	value_type;
		typedef std::size_t 						    	size_type;
		typedef std::ptrdiff_t 						    	difference_type;
		typedef Compare 							    	key_compare;
		typedef Allocator 							    	allocator_type;
		typedef value_type& 						    	reference;
		typedef const value_type& 					    	const_reference;
		typedef typename Allocator::pointer 		    	pointer;
		typedef typename Allocator::const_pointer	    	const_pointer;
        typedef TreeIterator<TreeNode<value_type>, false>	iterator;
        typedef	TreeIterator<TreeNode<value_type>, true> 	const_iterator;
        typedef	ft::reverse_iterator<iterator> 				reverse_iterator;
        typedef	ft::reverse_iterator<const_iterator>    	const_reverse_iterator;

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
		typedef RBTree<value_type, value_compare, allocator_type> tree_type;

		tree_type 			_tree;
		size_type 			_size;
		node_type*			_first;
		node_type*			_last;

	public:
		map():
            _tree(),
			_size(),
			_first(&_tree._end),
			_last(&_tree._end) {}

		explicit map( const Compare& comp,
					  const Allocator& alloc = Allocator() ):
					  _tree(value_compare(comp), alloc),
					  _size(),
                      _first(&_tree._end),
                      _last(&_tree._end) {}

		template< class InputIt >
		map( InputIt first, InputIt last,
			 const Compare& comp = Compare(),
			 const Allocator& alloc = Allocator() ):
			 _tree(value_compare(comp), alloc),
			 _size(),
             _first(&_tree._end),
             _last(&_tree._end)
         { insert(first, last); }

		map( const map& other ):
			_tree(other._tree),
			_size(other._size),
			_first(_tree._root->advanced_left()),
			_last(_tree._root->advanced_right()) {}

		map& operator=( const map& other )
		{
			if (this == &other)
				return *this;
			_tree = other._tree;
			_size = other._size;
			_first = _tree._root->advanced_left();
			_last = _tree._root->advanced_right();
            return *this;
		}

		allocator_type get_allocator() const
		{ return _tree._alloc; }

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
        { return iterator(&_tree._end); }

        const_iterator end() const
        { return const_iterator(&_tree._end); }

        reverse_iterator rbegin()
        { return reverse_iterator(iterator(_last)); }

        const_reverse_iterator rbegin() const
        { return const_reverse_iterator(const_iterator(_last)); }

        reverse_iterator rend()
        { return reverse_iterator(iterator(&_tree._end)); }

        const_reverse_iterator rend() const
        { return const_reverse_iterator(const_iterator(&_tree._end)); }

		bool empty() const
		{ return !_size; }

		size_type size() const
		{ return _size; }

		size_type max_size() const
		{ return std::numeric_limits<difference_type>::max(); }

		void clear()
		{
			_tree = tree_type();
            _first = &_tree._end;
            _last = &_tree._end;
			_size = 0;
		}

		ft::pair<iterator, bool> insert(const value_type& value)
		{
            node_type* pos = _find(value.first, _root);
            if (!pos->is_nil() &&
                !_key_comp(pos->value->first, value.first) &&
                !_key_comp(value.first, pos->value->first))
                return ft::make_pair(iterator(pos), false);
			node_type* new_node = _alloc_and_init_node(pos->is_nil() ? &_end : pos, true, value);
            if (!pos->is_nil())
            {
                if (!_key_comp(pos->value->first, value.first))
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
            if (!pos->is_nil() &&
                !_key_comp(pos->value->first, value.first) &&
                !_key_comp(value.first, pos->value->first))
                return iterator(pos);
            node_type* new_node = _alloc_and_init_node(pos->is_nil() ? &_end : pos, true, value);
            if (!pos->is_nil())
            {
                if (!_key_comp(pos->value->first, value.first))
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
                _node_swap(ptr, ptr->right->advanced_left());
            _delete_one_child(ptr);
            if (_root->is_nil())
            {
                _first = &_end;
                _last = &_end;
            }
            else
            {
                _first = _root->advanced_left();
                _last = _root->advanced_right();
            }
            --_size;
        }

		void erase( iterator first, iterator last )
        {
            node_type* ptr;

            for (iterator it = first; it != last;)
            {
                ptr = it._ptr;
                ++it;
                if (!ptr->left->is_nil() && !ptr->right->is_nil())
                    _node_swap(ptr, ptr->right->advanced_left());
                _delete_one_child(ptr);
                --_size;
            }
            if (_root->is_nil())
            {
                _first = &_end;
                _last = &_end;
            }
            else
            {
                _first = _root->advanced_left();
                _last = _root->advanced_right();
            }
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
            std::swap(_key_comp, other._key_comp);
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
				!_key_comp(ptr->value->first, key) &&
				!_key_comp(key, ptr->value->first))
                return iterator(ptr);
            else
			    return end();
		}

		const_iterator find( const Key& key ) const
		{
            node_type* ptr = _find(key, _root);
            if (!ptr->is_nil() &&
				!_key_comp(ptr->value->first, key) &&
				!_key_comp(key, ptr->value->first))
                return const_iterator(ptr);
            else
                return end();
		}

		ft::pair<iterator,iterator> equal_range( const Key& key )
		{
            iterator pos = lower_bound(key);
            if (pos == end())
                return ft::make_pair(pos, pos);
            if (!_key_comp(key, pos->first) && !_key_comp(pos->first, key))
                return ft::make_pair(pos, ++pos);
            else
                return ft::make_pair(pos, pos);
        }

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
		{
            const_iterator pos = lower_bound(key);
            if (pos == end())
                return ft::make_pair(pos, pos);
            if (!_key_comp(key, pos->first) && !_key_comp(pos->first, key))
                return ft::make_pair(pos, ++pos);
            else
                return ft::make_pair(pos, pos);
        }

		iterator lower_bound( const Key& key )
		{
			node_type* tmp = _root;
			node_type* prev = nullptr;
			while (tmp != node_type::nil())
			{
				if (_key_comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
                else if (!_key_comp(tmp->value->first, key))
                    return iterator(tmp);
				else
					tmp = tmp->right;
			}
			return prev && _key_comp(key, prev->value->first) ? iterator(prev) : end();
		}

		const_iterator lower_bound( const Key& key ) const
		{
			node_type* tmp = _root;
			node_type* prev = nullptr;
			while (tmp != node_type::nil())
			{
				if (_key_comp(key, tmp->value->first))
				{
					prev = tmp;
					tmp = tmp->left;
				}
                else if (!_key_comp(tmp->value->first, key))
                    return const_iterator(tmp);
				else
					tmp = tmp->right;
			}
			return prev && _key_comp(key, prev->value->first) ? const_iterator(prev) : end();
		}

		iterator upper_bound( const Key& key )
		{
            iterator pos = lower_bound(key);
            if (pos == end())
                return pos;
            if (!_key_comp(key, pos->first) && !_key_comp(pos->first, key))
                return ++pos;
            else
                return pos;
		}

		const_iterator upper_bound( const Key& key ) const
		{
            const_iterator pos = lower_bound(key);
            if (pos == end())
                return pos;
            if (!_key_comp(key, pos->first) && !_key_comp(pos->first, key))
                return ++pos;
            else
                return pos;
		}

		value_compare value_comp() const
		{ return value_compare(_key_comp); }

		key_compare key_comp() const
		{ return _key_comp; }

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
