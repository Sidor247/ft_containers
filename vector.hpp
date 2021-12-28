/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwhis <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 13:37:47 by cwhis             #+#    #+#             */
/*   Updated: 2021/12/28 17:38:02 by cwhis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <limits>
#include <cstring>

namespace ft {

template<
    class T,
    class Allocator = std::allocator<T>
> class vector
{
private:
	T*			_arr;
	size_t		_size;
	size_t		_cap;
	Allocator	_allocator;
public:
	vector(): 
		_arr(nullptr),
		_size(0),
		_cap(0),
		_allocator(std::allocator<T>()) {}

	explicit	vector( const Allocator& alloc ):
					_arr(nullptr),
					_size(0),
					_cap(0),
					_allocator(alloc) {}

	explicit	vector( size_t count,
                	const T& value = T(),
                	const Allocator& alloc = Allocator()):
					_size(count),
					_cap(count),
					_allocator(alloc)
	{
		_arr = _allocator.allocate(count);
		size_t i = 0;
		try
		{
			for (; i < count; ++i)
				_allocator.construct(_arr + i, value);
		}
		catch (...)
		{
			for (size_t j = 0; j < i; ++j)
				_allocator.destroy(_arr + j);
			_allocator.deallocate(_arr, count);
			throw;
		}
	}

	// template< class InputIt >
	// 			vector( InputIt first, InputIt last,
    //     			const Allocator& alloc = Allocator() ):
	// 				_allocator(alloc)
	// {

	// }

	vector( const vector& other ):
		_size(other._size),
		_cap(other._cap),
		_allocator(other._allocator)
	{
		_arr = _allocator.allocate(_cap);
		try
		{
			std::uninitialized_copy(other._arr, other.arr + _size, _arr);
		}
		catch(...)
		{
			_allocator.deallocate(_arr, _cap);
			throw;
		}
	}

	~vector()
	{
		for (size_t i = 0; i < _size; ++i)
			_allocator.destroy(_arr + i);
		_allocator.deallocate(_arr, _cap);
	}

	vector& operator=( const vector& other )
	{
		if (this == &other)
			return *this;
		T* newarr = other._allocator.allocate(other._cap);
		try
		{
			std::uninitialized_copy(other._arr, other.arr + other._size, newarr);
		}
		catch(...)
		{
			_allocator.deallocate(_arr, other._cap);
			throw;
		}
		_arr = newarr;
		_size = other._size;
		_cap = other._cap;
		_allocator = other._allocator;
		return *this;
	}

	void assign( size_t count, const T& value )
	{
		T* newarr = _allocator.allocate(count);
		size_t i = 0;
		try
		{
			for (; i < count; ++i)
				_allocator.construct(newarr + i, value);
		}
		catch(...)
		{
			for (size_t j = 0; j < i; ++j)
				_allocator.destroy(newarr + j);
			_allocator.deallocate(newarr, count);
			throw;
		}
		for (i = 0; i < _size; ++i)
			_allocator.destroy(_arr + i);
		_allocator.deallocate(_arr, _cap);
		_arr = newarr;
		_size = count;
		_cap = count;
	}

	// template< class InputIt >
	// void assign( InputIt first, InputIt last );

	Allocator get_allocator() const
	{
		return _allocator;
	}

	T&	at( size_t pos )
	{
		if (pos >= _size)
			throw std::out_of_range("out_of_range");
		return _arr[pos];
	}

	const T&	at( size_t pos ) const
	{
		if (pos >= _size)
			throw std::out_of_range("out_of_range");
		return _arr[pos];
	}

	T&	operator[]( size_t pos )
	{
		return _arr[pos];
	}

	const T&	operator[]( size_t pos ) const
	{
		return _arr[pos];
	}

	T&	front()
	{
		return _arr[0];
	}

	const T&	front() const
	{
		return _arr[0];	
	}

	T&	back()
	{
		return _arr[_size - 1];
	}

	const T&	back() const
	{
		return _arr[_size - 1];
	}

	T*	data()
	{
		return _arr;
	}

	const T*	data() const
	{
		return _arr;
	}

	bool empty() const
	{
		return !(_size);
	}

	size_t size() const
	{
		return _size;
	}

	size_t max_size() const
	{
		return std::numeric_limits<size_t>::max();
	}

	void reserve( size_t new_cap )
	{
		if (new_cap <= _cap)
			return;
		if (new_cap > max_size())
			throw std::length_error("length_error");
		T* newarr = _allocator.allocate(new_cap);
		try
		{
			std::uninitialized_copy(_arr, _arr + _size, newarr);
		}
		catch(...)
		{
			_allocator.deallocate(newarr, new_cap);
			throw;
		}
		_allocator.deallocate(_arr, _cap);
		_arr = newarr;
		_cap = new_cap;
	}

	size_t capacity() const
	{
		return _cap;
	}

	void clear()
	{
		for (size_t i = 0; i < _size; ++i)
			_allocator.destroy(_arr + i);
		_size = 0;
	}

	void push_back( const T& value )
	{
		if (_size == _cap)
			reserve(2 * _cap);
		_allocator.construct(_arr + _size++, value);
	}

	void pop_back()
	{
		_allocator.destroy(_arr + --_size);
	}

	void resize( size_t count, T value = T() )
	{
		if (count > _cap)
			reserve(count);
		if (count > _size)
			for (size_t i = _size; i < count; ++i)
				_allocator.construct(_arr + i, value);
		if (count < _size)
			for (size_t i = count; i < _size; ++i)
				_allocator.destroy(_arr + i);
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

}

#endif