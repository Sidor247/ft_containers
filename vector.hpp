#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <limits>
#include <cstring>
#include <iterator>
#include <iostream>
#include "TypeTraits.hpp"
#include "ReverseIterator.hpp"

namespace ft {

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
		value_type*		_arr;

		template<bool IsConst>
		class _common_iterator
		{
		public:
			typedef std::ptrdiff_t									difference_type;
			typedef T												value_type;
			typedef typename conditional<IsConst,const T*,T*>::type	pointer;
			typedef typename conditional<IsConst,const T&,T&>::type	reference;
			typedef std::random_access_iterator_tag					iterator_category;

			_common_iterator(pointer ptr): _ptr(ptr) {}
			_common_iterator(const _common_iterator& src): _ptr(src._ptr) {}
			~_common_iterator() {}

			_common_iterator&	operator=(const _common_iterator& src)
			{
				_ptr = src._ptr;
				return *this;
			}

			reference	operator*() const
			{
				return *_ptr;
			}

			reference	operator->() const
			{
				return _ptr;
			}

			reference	operator[](difference_type n) const
			{
				return *(*this + n);
			}

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

			_common_iterator&	operator+=(difference_type rhs)
			{
				_ptr += rhs;
				return *this;
			}

			_common_iterator&	operator-=(difference_type rhs)
			{
				_ptr -= rhs;
				return *this;
			}

			_common_iterator	operator+(difference_type rhs)
			{
				_common_iterator tmp = *this;
				return tmp += rhs;
			}

	friend	_common_iterator	operator+(difference_type lhs, _common_iterator rhs)
			{ return rhs += lhs; }

			_common_iterator	operator-(difference_type rhs)
			{
				_common_iterator tmp = *this;
				return tmp -= rhs;
			}

			difference_type	operator-(const _common_iterator& rhs)
			{ return _ptr - rhs._ptr; }

			bool	operator==(const _common_iterator& rhs) const
			{ return _ptr == rhs._ptr; }

			bool	operator!=(const _common_iterator& rhs) const
			{ return _ptr != rhs._ptr; }

			bool	operator<(_common_iterator& rhs) const
			{ return rhs - *this > 0; }

			bool	operator>(_common_iterator& rhs) const
			{ return rhs < *this; }

			bool	operator>=(_common_iterator& rhs) const
			{ return !(*this < rhs); }

			bool	operator<=(_common_iterator& rhs) const
			{ return !(*this > rhs); }

		private:
			pointer _ptr;
		};

		void destroy_and_dealloc()
		{
			for (size_type i = 0; i < _size; ++i)
				_alloc.destroy(_arr + i);
			_alloc.deallocate(_arr, _cap);
		}

		T*	alloc_and_init(size_type count, const T& value)
		{
			T* newarr = _alloc.allocate(count);
			size_type i = 0;
			try
			{
				for (; i < count; ++i)
					_alloc.construct(newarr + i, value);
			}
			catch (...)
			{
				for (size_type j = 0; j < i; ++i)
					_alloc.destroy(newarr + j);
				_alloc.deallocate(newarr, count);
				throw;
			}
			return newarr;
		}

		T*	_alloc_and_copy(const vector& src) {
			Allocator alloc = src.get_allocator();
			T *newarr = alloc.allocate(src._cap);
			size_type i = 0;
			try {
				for (; i < src._size; ++i)
					alloc.construct(newarr + i, src._arr[i]);
			}
			catch (...) {
				for (size_type j = 0; j < i; ++i)
					alloc.destroy(newarr + j);
				alloc.deallocate(newarr, src._cap);
				throw;
			}
			return newarr;
		}

		template<class InputIt>
		void _reserve_iter_impl(InputIt, InputIt, std::input_iterator_tag) {}

		template<class ForwardIt>
		void _reserve_iter_impl(ForwardIt first, ForwardIt last, std::forward_iterator_tag)
		{ reserve(std::distance(first, last)); }

		template<class Iter>
		void _reserve_impl(Iter first, typename enable_if<!is_pointer<Iter>::value, Iter>::type last)
		{ _reserve_iter_impl(first, last, typename iterator_traits<Iter>::iterator_category()); }

		template<class Pointer>
		void _reserve_impl(Pointer first, typename enable_if<is_pointer<Pointer>::value, Pointer>::type last)
		{ reserve(last - first); }

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
						_arr(alloc_and_init(count, value)) {}

		template<typename InputIt>
		vector( InputIt first,
				typename enable_if<!is_integral<InputIt>::value, InputIt>::type last,
				const Allocator& alloc = Allocator()):
				_alloc(alloc),
				_size(0u),
				_cap(0u),
				_arr(nullptr)
		{
			_reserve_impl(first, last);
			try
			{
				for (InputIt it = first; it != last; ++it)
				{ push_back(*it); }
			}
			//?
			catch (...)
			{
				destroy_and_dealloc();
				throw;
			}
		}

		template<typename Integral>
		vector( Integral first,
				typename enable_if<is_integral<Integral>::value, Integral>::type last,
				const Allocator& alloc = Allocator()):
				_alloc(alloc),
				_size(static_cast<size_type>(first)),
				_cap(static_cast<size_type>(first)),
				_arr(alloc_and_init(static_cast<size_type>(first), static_cast<value_type>(last))) {}

		vector( const vector& other ):
			_size(other._size),
			_cap(other._cap),
			_alloc(other._alloc),
			_arr(alloc_and_copy(other)) {}

		~vector()
		{ destroy_and_dealloc(); }

		vector& operator=( const vector& other )
		{
			if (this == &other)
				return *this;
			T* newarr = _alloc_and_copy(other);
			destroy_and_dealloc();
			_arr = newarr;
			_size = other._size;
			_cap = other._cap;
			_alloc = other._alloc;
			return *this;
		}

		void assign( size_type count, const T& value )
		{
			T* newarr = alloc_and_init(count, value);
			destroy_and_dealloc();
			_arr = newarr;
			_size = count;
			_cap = count;
		}

		template< class InputIt >
		void assign( InputIt first, InputIt last )
		{ swap(vector(first, last)); }

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
			T* newarr = _alloc_and_copy(*this);
			destroy_and_dealloc();
			_arr = newarr;
			_cap = new_cap;
		}

		size_type capacity() const
		{ return _cap; }

		void clear()
		{
			for (size_type i = 0; i < _size; ++i)
				_alloc.destroy(_arr + i);
			_size = 0;
		}

	//	iterator insert( iterator pos, const T& value )
	//	{
	//		if (!_cap)
	//			reserve(1u);
	//		if (_size == _cap)
	//			reserve(2 * _cap);
	//
	//	}
	//
	//	void insert( iterator pos, size_type count, const T& value )
	//	{
	//
	//	}
	//
	//	template< class InputIt >
	//	void insert( iterator pos, InputIt first, InputIt last )
	//	{
	//
	//	}
	//
	//	iterator erase( iterator pos )
	//	{
	//
	//	}
	//
	//	iterator erase( iterator first, iterator last )
	//	{
	//
	//	}

		void push_back( const T& value )
		{
			if (!_size)
				reserve(1u);
			if (_size == _cap)
				reserve(2 * _cap);
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
				for (size_type i = count; i < _size; ++i)
					_alloc.destroy(_arr + i);
			_size = count;
		}

		void swap( vector& other )
		{
			std::swap(_alloc, other._alloc);
			std::swap(_arr, other._arr);
			std::swap(_size, other._size);
			std::swap(_cap, other._cap);
		}
	};

	//template< class T, class Alloc >
	//bool operator==( const std::vector<T,Alloc>& lhs,
	//                 const std::vector<T,Alloc>& rhs );
	//
	//template< class T, class Alloc >
	//bool operator!=( const std::vector<T,Alloc>& lhs,
	//                 const std::vector<T,Alloc>& rhs );
	//
	//template< class T, class Alloc >
	//bool operator<( const std::vector<T,Alloc>& lhs,
	//                const std::vector<T,Alloc>& rhs );
	//
	//template< class T, class Alloc >
	//bool operator<=( const std::vector<T,Alloc>& lhs,
	//                 const std::vector<T,Alloc>& rhs );
	//
	//template< class T, class Alloc >
	//bool operator>( const std::vector<T,Alloc>& lhs,
	//                const std::vector<T,Alloc>& rhs );
	//
	//template< class T, class Alloc >
	//bool operator>=( const std::vector<T,Alloc>& lhs,
	//                 const std::vector<T,Alloc>& rhs );
	//
	//template< class T, class Alloc >
	//void swap( std::vector<T,Alloc>& lhs,
	//           std::vector<T,Alloc>& rhs );
}

#endif