/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwhis <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 19:18:28 by cwhis             #+#    #+#             */
/*   Updated: 2022/01/02 15:08:28 by cwhis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <limits>
#include <cstring>
#include <iterator>
#include "Utility.hpp"

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
	T*				_arr;
	size_type		_size;
	size_type		_cap;
	Allocator		_alloc;

	template<bool IsConst>
	class common_iterator
	{
	public:
		typedef std::ptrdiff_t										difference_type;
		typedef T													value_type;
		typedef typename ft::conditional<IsConst,const T*,T*>::type	pointer;
		typedef typename ft::conditional<IsConst,const T&,T&>::type	reference;
		typedef std::random_access_iterator_tag						iterator_category;
		
		common_iterator(pointer ptr): _ptr(ptr) {}
		common_iterator(const common_iterator& src): _ptr(src._ptr) {}
		~common_iterator() {}

		common_iterator&	operator=(const common_iterator& src)
		{
			if (this == &src)
				return *this;
			_ptr = src._ptr;
		}

		reference	operator*()
		{
			return *_ptr;
		}

		reference	operator->()
		{
			return _ptr;
		}

		reference	operator[](difference_type n)
		{
			return *(*this + n);
		}

		common_iterator&	operator++()
		{
			++_ptr;
			return *this;
		}

		common_iterator operator++(int)
		{
			common_iterator copy(*this);
			++_ptr;
			return copy;
		}

		common_iterator&	operator--()
		{
			--_ptr;
			return *this;
		}

		common_iterator operator--(int)
		{
			common_iterator copy(*this);
			--_ptr;
			return copy;
		}

		common_iterator&	operator+=(difference_type rhs)
		{
			_ptr += rhs;
			return *this;
		}

		common_iterator&	operator-=(difference_type rhs)
		{
			_ptr -= rhs;
			return *this;
		}

		common_iterator	operator+(difference_type rhs)
		{
			common_iterator tmp = *this;
			return tmp += rhs;
		}

friend	common_iterator operator+(difference_type lhs, common_iterator rhs)
		{
			return rhs + lhs;
		}

		common_iterator	operator-(difference_type rhs)
		{
			common_iterator tmp = *this;
			return tmp -= rhs;
		}		

		difference_type	operator-(const common_iterator& rhs)
		{
			return _ptr - rhs._ptr;
		}

		bool	operator==(const common_iterator& rhs)
		{
			return _ptr == rhs._ptr;
		}

		bool	operator!=(const common_iterator& rhs)
		{
			return _ptr != rhs._ptr;
		}

		bool	operator<(common_iterator& rhs)
		{
			return rhs - *this > 0;
		}

		bool	operator>(common_iterator& rhs)
		{
			return rhs < *this;
		}

		bool	operator>=(common_iterator& rhs)
		{
			return !(*this < rhs);
		}

		bool	operator<=(common_iterator& rhs)
		{
			return !(*this > rhs);
		}

	private:
		pointer _ptr;
	};

public:
	typedef common_iterator<false>					iterator;
	typedef common_iterator<true>					const_iterator;
	typedef std::reverse_iterator<iterator>			reverse_iterator;
	typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
	
	vector(): 
		_arr(nullptr),
		_size(0u),
		_cap(0u),
		_alloc(std::allocator<T>()) {}

	explicit	vector( const Allocator& alloc ):
					_arr(nullptr),
					_size(0u),
					_cap(0u),
					_alloc(alloc) {}

	explicit	vector( size_type count,
                	const T& value = T(),
                	const Allocator& alloc = Allocator()):
					_size(count),
					_cap(count),
					_alloc(alloc)
	{
		_arr = _alloc.allocate(count);
		size_type i = 0;
		try
		{
			for (; i < count; ++i)
				_alloc.construct(_arr + i, value);
		}
		catch (...)
		{
			for (size_type j = 0; j < i; ++j)
				_alloc.destroy(_arr + j);
			_alloc.deallocate(_arr, count);
			throw;
		}
	}

	template< class InputIt >
	vector( InputIt first, InputIt last,
		const Allocator& alloc = Allocator() ):
		_alloc(alloc)
	{
		//is_integral...
		size_type count = static_cast<size_type>(first);
		value_type value = static_cast<value_type>(last);
		_arr = _alloc.allocate(count);
		size_type i = 0;
		try
		{
			for (; i < count; ++i)
				_alloc.construct(_arr + i, value);
		}
		catch (...)
		{
			for (size_type j = 0; j < i; ++j)
				_alloc.destroy(_arr + j);
			_alloc.deallocate(_arr, count);
			throw;
		}
		_size = count;
		_cap = count;
	}

	vector( const vector& other ):
		_size(other._size),
		_cap(other._cap),
		_alloc(other._alloc)
	{
		_arr = _alloc.allocate(_cap);
		try
		{
			std::uninitialized_copy(other._arr, other.arr + _size, _arr);
		}
		catch(...)
		{
			_alloc.deallocate(_arr, _cap);
			throw;
		}
	}

	~vector()
	{
		for (size_type i = 0; i < _size; ++i)
			_alloc.destroy(_arr + i);
		_alloc.deallocate(_arr, _cap);
	}

	vector& operator=( const vector& other )
	{
		if (this == &other)
			return *this;
		T* newarr = other._alloc.allocate(other._cap);
		try
		{
			std::uninitialized_copy(other._arr, other.arr + other._size, newarr);
		}
		catch(...)
		{
			_alloc.deallocate(_arr, other._cap);
			throw;
		}
		for (size_type i = 0; i < _size; ++i)
			_alloc.destroy(_arr + i);
		_alloc.deallocate(_arr, _cap);
		_arr = newarr;
		_size = other._size;
		_cap = other._cap;
		_alloc = other._alloc;
		return *this;
	}

	void assign( size_type count, const T& value )
	{
		T* newarr = _alloc.allocate(count);
		size_type i = 0;
		try
		{
			for (; i < count; ++i)
				_alloc.construct(newarr + i, value);
		}
		catch(...)
		{
			for (size_type j = 0; j < i; ++j)
				_alloc.destroy(newarr + j);
			_alloc.deallocate(newarr, count);
			throw;
		}
		for (i = 0; i < _size; ++i)
			_alloc.destroy(_arr + i);
		_alloc.deallocate(_arr, _cap);
		_arr = newarr;
		_size = count;
		_cap = count;
	}

	template< class InputIt >
	void assign( InputIt first, InputIt last )
	{

	}

	allocator_type get_allocator() const
	{
		return _alloc;
	}

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
	{
		return _arr[pos];
	}

	const_reference operator[]( size_type pos ) const
	{
		return _arr[pos];
	}

	reference		front()
	{
		return _arr[0];
	}

	const_reference	front() const
	{
		return _arr[0];	
	}

	reference		back()
	{
		return _arr[_size - 1];
	}

	const_reference	back() const
	{
		return _arr[_size - 1];
	}

	T*			data()
	{
		return _arr;
	}

	const T*	data() const
	{
		return _arr;
	}

	iterator 		begin()
	{
		return iterator(_arr);
	}

	const_iterator	begin() const
	{
		return const_iterator(_arr);
	}

	iterator 		end()
	{
		return iterator(_arr + _size);
	}

	const_iterator	end() const
	{
		return const_iterator(_arr + _size);
	}

	reverse_iterator 		rbegin()
	{

	}

	const_reverse_iterator	rbegin() const
	{

	}

	reverse_iterator		rend()
	{

	}

	const_reverse_iterator	rend() const
	{

	}

	bool empty() const
	{
		return !(_size);
	}

	size_type size() const
	{
		return _size;
	}

	size_type max_size() const
	{
		return std::numeric_limits<difference_type>::max();
	}

	void reserve( size_type new_cap )
	{
		if (new_cap <= _cap)
			return;

		if (new_cap > max_size())
			throw std::length_error("length_error");
		T* newarr = _alloc.allocate(new_cap);
		try
		{
			std::uninitialized_copy(_arr, _arr + _size, newarr);
		}
		catch(...)
		{
			_alloc.deallocate(newarr, new_cap);
			throw;
		}
		_alloc.deallocate(_arr, _cap);
		_arr = newarr;
		_cap = new_cap;
	}

	size_type capacity() const
	{
		return _cap;
	}

	void clear()
	{
		for (size_type i = 0; i < _size; ++i)
			_alloc.destroy(_arr + i);
		_size = 0;
	}

	iterator insert( iterator pos, const T& value )
	{
		if (!_cap)
			reserve(1u);
		if (_size == _cap)
			reserve(2 * _cap);
		
	}

	void insert( iterator pos, size_type count, const T& value )
	{

	}

	template< class InputIt >
	void insert( iterator pos, InputIt first, InputIt last )
	{

	}

	iterator erase( iterator pos )
	{

	}

	iterator erase( iterator first, iterator last )
	{

	}

	void push_back( const T& value )
	{
		if (!_size)
			reserve(1u);
		if (_size == _cap)
			reserve(2 * _cap);
		_alloc.construct(_arr + _size++, value);
	}

	void pop_back()
	{
		_alloc.destroy(_arr + --_size);
	}

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
		char	buf[sizeof(vector)];
		std::memcpy(buf, this, sizeof(vector));
		std::memcpy(this, &other, sizeof(vector));
		std::memcpy(&other, buf, sizeof(vector));
	}
};

template< class T, class Alloc >
bool operator==( const std::vector<T,Alloc>& lhs,
                 const std::vector<T,Alloc>& rhs );

template< class T, class Alloc >
bool operator!=( const std::vector<T,Alloc>& lhs,
                 const std::vector<T,Alloc>& rhs );

template< class T, class Alloc >
bool operator<( const std::vector<T,Alloc>& lhs,
                const std::vector<T,Alloc>& rhs );

template< class T, class Alloc >
bool operator<=( const std::vector<T,Alloc>& lhs,
                 const std::vector<T,Alloc>& rhs );

template< class T, class Alloc >
bool operator>( const std::vector<T,Alloc>& lhs,
                const std::vector<T,Alloc>& rhs );

template< class T, class Alloc >
bool operator>=( const std::vector<T,Alloc>& lhs,
                 const std::vector<T,Alloc>& rhs );

template< class T, class Alloc >
void swap( std::vector<T,Alloc>& lhs,
           std::vector<T,Alloc>& rhs );
}

#endif