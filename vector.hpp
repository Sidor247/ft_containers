#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <limits>
#include <iterator>
#include <iostream>
#include "algorithm.hpp"
#include "type_traits.hpp"
#include "reverse_iterator.hpp"

namespace ft
{

	template<
		class T,
		class Allocator = std::allocator<T>
	> class vector
	{
	public:
		typedef T 										value_type;
		typedef Allocator								allocator_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;

	private:
		allocator_type	_alloc;
		size_type		_size;
		size_type		_cap;
		pointer			_arr;

		template<bool IsConst>
		class _common_iterator : public std::iterator<
			std::random_access_iterator_tag,
			T,
			std::ptrdiff_t,
			typename ft::conditional<IsConst, const T*, T*>::type,
			typename ft::conditional<IsConst, const T&, T&>::type>
		{
		public:
			_common_iterator(): _ptr(nullptr) {}
			_common_iterator(pointer ptr): _ptr(ptr) {}
			_common_iterator(const _common_iterator& src): _ptr(src._ptr) {}
		   ~_common_iterator() {}

			_common_iterator&	operator=(const _common_iterator& src)
			{
				_ptr = src._ptr;
				return *this;
			}

			reference	operator*() const
			{ return *_ptr; }

			pointer		operator->() const
			{ return _ptr; }

			reference	operator[](difference_type n) const
			{ return *(*this + n); }

			_common_iterator&	operator++()
			{
				++_ptr;
				return *this;
			}

			_common_iterator operator++(int)
			{
				_common_iterator copy(*this);
				++_ptr;
				return copy;
			}

			_common_iterator&	operator+=(difference_type rhs)
			{
				_ptr += rhs;
				return *this;
			}

			_common_iterator	operator+(difference_type rhs) const
			{
				_common_iterator tmp = *this;
				return tmp += rhs;
			}

			_common_iterator&	operator--()
			{
				--_ptr;
				return *this;
			}

			_common_iterator operator--(int)
			{
				_common_iterator copy(*this);
				--_ptr;
				return copy;
			}

			_common_iterator&	operator-=(difference_type rhs)
			{
				_ptr -= rhs;
				return *this;
			}

			_common_iterator	operator-(difference_type rhs) const
			{
				_common_iterator copy(*this);
				return copy -= rhs;
			}

			operator _common_iterator<true>()
			{ return _common_iterator<true>(_ptr); }

	friend	bool operator==(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return lhs._ptr == rhs._ptr; }

	friend	bool operator!=(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return lhs._ptr != rhs._ptr; }

	friend	bool operator< (const _common_iterator& lhs, const _common_iterator& rhs)
			{ return rhs - lhs > 0; }

	friend	bool operator> (const _common_iterator& lhs, const _common_iterator& rhs)
			{ return rhs < lhs; }

	friend	bool operator>=(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return !(lhs < rhs); }

	friend	bool operator<=(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return !(lhs > rhs); }

	friend	_common_iterator	operator+(difference_type lhs, _common_iterator rhs)
			{ return rhs += lhs; }

	friend	difference_type		operator-(const _common_iterator& lhs, const _common_iterator& rhs)
			{ return lhs._ptr - rhs._ptr; }

		private:
			pointer _ptr;
		};

		void _destroy_range(T* start, T* end)
		{
			for (T* ptr = start; ptr != end; ++ptr)
				_alloc.destroy(ptr);
		}

		void _destroy_and_dealloc()
		{
			if (!_arr) return;
			_destroy_range(_arr, _arr + _size);
			_alloc.deallocate(_arr, _cap);
		}

		T*	_alloc_and_init(size_type count, const T& value)
		{
			if (!count)
				return nullptr;
			T* new_arr = _alloc.allocate(count);
			size_type i = 0;
			try
			{
				for (; i < count; ++i)
					_alloc.construct(new_arr + i, value);
			}
			catch (...)
			{
				_destroy_range(new_arr, new_arr + i);
				_alloc.deallocate(new_arr, count);
				throw;
			}
			return new_arr;
		}

		T*	_alloc_and_copy(T* arr)
		{
			if (!arr)
				return nullptr;
			T *new_arr = _alloc.allocate(_cap);
			size_type i = 0;
			try {
				for (; i < _size; ++i)
					_alloc.construct(new_arr + i, arr[i]);
			}
			catch (...)
			{
				_destroy_range(new_arr, new_arr + i);
				_alloc.deallocate(new_arr, _cap);
				throw;
			}
			return new_arr;
		}

		void _basic_exception_handler()
		{
			_alloc.deallocate(_arr, _cap);
			_size = 0;
			_cap = 0;
			_arr = nullptr;
		}

		template<class ForwardIt>
		void _reserve_iter_impl(ForwardIt first, ForwardIt last, std::forward_iterator_tag)
		{ reserve(std::distance(first, last)); }

		template<class InputIt>
		void _reserve_iter_impl(InputIt, InputIt, std::input_iterator_tag) {}

		template< class ForwardIt >
		void _insert_iter_impl(_common_iterator<false> pos, ForwardIt first, ForwardIt last, std::forward_iterator_tag)
		{
			difference_type insert_index = pos - begin();
			size_type count = std::distance(first, last);
			if (_size + count > _cap)
				reserve(_size + count);
			T* insert_ptr	= _arr + insert_index;
			T* left_ptr		= _arr + _size;
			T* right_ptr	= _arr + _size + count - 1;
			try
			{
				for (; left_ptr > insert_ptr; --left_ptr, --right_ptr)
				{
					_alloc.construct(right_ptr, left_ptr[-1]);
					_alloc.destroy(left_ptr - 1);
				}
				for (ForwardIt it = first; it != last; ++it, ++left_ptr)
					_alloc.construct(left_ptr, *it);
			}
			catch (...)
			{
				_destroy_range(_arr, left_ptr);
				_destroy_range(right_ptr, _arr + _size + count);
				_basic_exception_handler();
			}
			_size += count;
		}

		template< class InputIt >
		void _insert_iter_impl(_common_iterator<false> pos, InputIt first, InputIt last, std::input_iterator_tag)
		{
			vector	to_insert(first, last);
			_insert_iter_impl(pos, to_insert.begin(), to_insert.end(),
								  typename ft::iterator_traits<iterator>::iterator_category());
		}

	public:
		typedef _common_iterator<false>					iterator;
		typedef _common_iterator<true>					const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		vector():
			_alloc(std::allocator<T>()),
			_size(0u),
			_cap(0u),
			_arr(nullptr) {}

		explicit	vector( const Allocator& alloc ):
						_alloc(alloc),
						_size(0u),
						_cap(0u),
						_arr(nullptr) {}

		explicit	vector( size_type count,
						const T& value = T(),
						const Allocator& alloc = Allocator()):
						_alloc(alloc),
						_size(count),
						_cap(count),
						_arr(_alloc_and_init(count, value)) {}

		template<typename InputIt>
		vector( InputIt first,
				typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last,
				const Allocator& alloc = Allocator()):
				_alloc(alloc),
				_size(0u),
				_cap(0u),
				_arr(nullptr)
		{
			_reserve_iter_impl(first, last, typename ft::iterator_traits<InputIt>::iterator_category());
			try
			{
				for (InputIt it = first; it != last; ++it)
				{ push_back(*it); }
			}
			catch (...)
			{
				_destroy_and_dealloc();
				throw;
			}
		}

		template<typename Integral>
		vector( Integral first,
				typename ft::enable_if<ft::is_integral<Integral>::value, Integral>::type last,
				const Allocator& alloc = Allocator()):
				_alloc(alloc),
				_size(static_cast<size_type>(first)),
				_cap(static_cast<size_type>(first)),
				_arr(_alloc_and_init(static_cast<size_type>(first), static_cast<value_type>(last))) {}

		vector( const vector& other ):
			_alloc(other._alloc),
			_size(other._size),
			_cap(other._cap),
			_arr(_alloc_and_copy(other._arr)) {}

	   ~vector()
		{ _destroy_and_dealloc(); }

		vector& operator=( const vector& other )
		{
			if (this == &other)
				return *this;
			T* new_arr = _alloc_and_copy(other._arr);
			_destroy_and_dealloc();
			_alloc = other._alloc;
			_size = other._size;
			_cap = other._cap;
			_arr = new_arr;
			return *this;
		}

		void assign( size_type count, const T& value )
		{ swap(vector(count, value, _alloc)); }

		template< class InputIt >
		void assign( InputIt first, InputIt last )
		{ swap(vector(first, last, _alloc)); }

		allocator_type get_allocator() const
		{ return _alloc; }

		reference		at( size_type pos )
		{
			if (pos >= _size)
				throw std::out_of_range("out_of_range");
			return _arr[pos];
		}

		const_reference	at( size_type pos ) const
		{
			if (pos >= _size)
				throw std::out_of_range("out_of_range");
			return _arr[pos];
		}

		reference		operator[]( size_type pos )
		{ return _arr[pos]; }

		const_reference operator[]( size_type pos ) const
		{ return _arr[pos]; }

		reference		front()
		{ return _arr[0]; }

		const_reference	front() const
		{ return _arr[0]; }

		reference		back()
		{ return _arr[_size - 1]; }

		const_reference	back() const
		{ return _arr[_size - 1]; }

		T*			data()
		{ return _arr; }

		const T*	data() const
		{ return _arr; }

		iterator 		begin()
		{ return iterator(_arr); }

		const_iterator	begin() const
		{ return const_iterator(_arr); }

		iterator 		end()
		{ return iterator(_arr + _size); }

		const_iterator	end() const
		{ return const_iterator(_arr + _size); }

		reverse_iterator 		rbegin()
		{ return reverse_iterator(iterator(_arr + _size - 1)); }

		const_reverse_iterator	rbegin() const
		{ return const_reverse_iterator(const_iterator(_arr + _size - 1)); }

		reverse_iterator		rend()
		{ return reverse_iterator(iterator(_arr - 1)); }

		const_reverse_iterator	rend() const
		{ return reverse_iterator(const_iterator(_arr - 1)); }

		bool empty() const
		{ return !(_size); }

		size_type size() const
		{ return _size; }

		size_type max_size() const
		{ return std::numeric_limits<difference_type>::max(); }

		void reserve( size_type new_cap )
		{
			if (new_cap <= _cap)
				return;
			if (new_cap > max_size())
				throw std::length_error("length_error");
			T* new_arr = _alloc.allocate(new_cap);
			size_type i = 0;
			try
			{
				for (; i < _size; ++i)
					_alloc.construct(new_arr + i, _arr[i]);
			}
			catch (...)
			{
				_destroy_range(new_arr, new_arr + i);
				_alloc.deallocate(new_arr, new_cap);
				throw;
			}
			_destroy_and_dealloc();
			_arr = new_arr;
			_cap = new_cap;
		}

		size_type capacity() const
		{ return _cap; }

		void clear()
		{
			_destroy_range(_arr, _arr + _size);
			_size = 0;
		}

		iterator insert( iterator pos, const T& value )
		{
			if (pos == end())
			{
				push_back(value);
				return (end() - 1);
			}
			difference_type insert_index = pos - begin();
			if (_size == _cap)
				reserve(_cap ? 2 * _cap : 1);
			T* insert_ptr = _arr + insert_index;
			T* tmp_ptr = _arr + _size;
			try
			{
				for (; tmp_ptr > insert_ptr; --tmp_ptr)
				{
					_alloc.construct(tmp_ptr, tmp_ptr[-1]);
					_alloc.destroy(tmp_ptr - 1);
				}
				_alloc.construct(tmp_ptr, value);
			}
			catch (...)
			{
				_destroy_range(_arr, tmp_ptr);
				_destroy_range(tmp_ptr + 1, _arr + _size + 1);
				_basic_exception_handler();
				throw;
			}
			++_size;
			return iterator(_arr + insert_index);
		}

		void insert( iterator pos, size_type count, const T& value )
		{
			difference_type insert_index = pos - begin();
			if (_size + count > _cap)
				reserve(_size + count);
			T* insert_ptr	= _arr + insert_index;
			T* first_ptr	= _arr + _size;
			T* last_ptr		= _arr + _size + count - 1;
			try
			{
				for (; first_ptr > insert_ptr; --first_ptr, --last_ptr)
				{
					_alloc.construct(last_ptr, first_ptr[-1]);
					_alloc.destroy(first_ptr - 1);
				}
				for (; first_ptr <= last_ptr; ++first_ptr)
					_alloc.construct(first_ptr, value);
			}
			catch (...)
			{
				_destroy_range(_arr, first_ptr);
				_destroy_range(last_ptr + 1, _arr + _size + count);
				_basic_exception_handler();
				throw;
			}
			_size += count;
		}

		template< class InputIt >
		void insert( iterator pos, InputIt first,
					 typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last )
		{
			_insert_iterator_impl(pos, first, last,
								typename ft::iterator_traits<InputIt>::iterator_category());
		}

		template< class Integral >
		void insert( iterator pos, Integral first,
					 typename ft::enable_if<ft::is_integral<Integral>::value, Integral>::type last )
		{ insert(pos, static_cast<size_type>(first), static_cast<value_type>(last)); }

		iterator erase( iterator pos )
		{
			T* erase_ptr = pos - begin() + _arr;
			T* back_ptr = _arr + _size - 1;
			_alloc.destroy(erase_ptr);
			--_size;
			try
			{
				for (; erase_ptr < back_ptr; ++erase_ptr)
				{
					_alloc.construct(erase_ptr, erase_ptr[1]);
					_alloc.destroy(erase_ptr + 1);
				}
			}
			catch (...)
			{
				_destroy_range(_arr, erase_ptr);
				_destroy_range(erase_ptr + 1, _arr + _size);
				_basic_exception_handler();
				throw;
			}
			return pos;
		}

		iterator erase( iterator first, iterator last )
		{
			size_type count = last - first;
			T* first_ptr = first - begin() + _arr;
			T* last_ptr = last - begin() + _arr;
			T* back_ptr = _arr + _size - 1;
			_destroy_range(first_ptr, last_ptr);
			try
			{
				for (; last_ptr <= back_ptr; ++last_ptr, ++first_ptr)
				{
					_alloc.construct(first_ptr, *last_ptr);
					_alloc.destroy(last_ptr);
				}
			}
			catch (...)
			{
				_destroy_range(_arr, first_ptr);
				_destroy_range(last_ptr, _arr + _size);
				_basic_exception_handler();
				throw;
			}
			_size -= count;
			return first;
		}

		void push_back( const T& value )
		{
			if (_size == _cap)
				reserve(_cap ? 2 * _cap : 1);
			_alloc.construct(_arr + _size++, value);
		}

		void pop_back()
		{ _alloc.destroy(_arr + --_size); }

		void resize( size_type count, T value = T() )
		{
			if (count > _cap)
				reserve(count);
			if (count > _size)
				for (size_type i = _size; i < count; ++i)
					_alloc.construct(_arr + i, value);
			if (count < _size)
				_destroy_range(_arr + count, _arr + _size);
			_size = count;
		}

		void swap( vector& other )
		{
			std::swap(_alloc, other._alloc);
			std::swap(_arr, other._arr);
			std::swap(_size, other._size);
			std::swap(_cap, other._cap);
		}


friend 	bool operator==(const vector& lhs, const vector& rhs )
		{ return lhs._size == rhs._size && ft::equal(lhs.begin(), lhs.end(), rhs.begin()); }

friend 	bool operator!=(const vector& lhs, const vector& rhs )
		{ return !(lhs == rhs); }

friend 	bool operator< (const vector& lhs, const vector& rhs )
		{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

friend 	bool operator> (const vector& lhs, const vector& rhs )
		{ return rhs < lhs; }

friend 	bool operator<=(const vector& lhs, const vector& rhs )
		{ return !(rhs > lhs); }

friend 	bool operator>=(const vector& lhs, const vector& rhs )
		{ return !(rhs < lhs); }

friend	void swap( vector& lhs, vector& rhs )
		{ lhs.swap(rhs); }
	};

}

#endif