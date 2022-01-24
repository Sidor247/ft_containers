#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "type_traits.hpp"

namespace ft
{
	template< class Iter >
	class reverse_iterator
	{
	public:
		typedef Iter 													iterator_type;
		typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
		typedef	typename ft::iterator_traits<Iter>::value_type			value_type;
		typedef	typename ft::iterator_traits<Iter>::difference_type		difference_type;
		typedef	typename ft::iterator_traits<Iter>::pointer 			pointer;
		typedef	typename ft::iterator_traits<Iter>::reference 			reference;

		reverse_iterator(): current(Iter()) {}
		explicit	reverse_iterator(iterator_type it): current(it) {}
		reverse_iterator(const reverse_iterator& rev_it): current(rev_it.current) {}

		iterator_type base() const
		{ return current; }

		reference operator*() const
		{ return current.operator*(); }

		reference operator->() const
		{ return current.operator->(); }

		reference operator[](difference_type n) const
		{ return current[-n-1]; }

		reverse_iterator& operator++()
		{
			--current;
			return *this;
		}

		reverse_iterator  operator++(int)
		{
			reverse_iterator copy(current--);
			return copy;
		}

		reverse_iterator& operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}

		reverse_iterator operator+(difference_type n) const
		{ return reverse_iterator(current - n); }

		reverse_iterator& operator--()
		{
			++current;
			return *this;
		}

		reverse_iterator  operator--(int)
		{
			reverse_iterator copy(current--);
			return copy;
		}

		reverse_iterator& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}

		reverse_iterator operator-(difference_type n) const
		{ return reverse_iterator(current + n); }

friend	bool operator==	(const reverse_iterator& lhs,
						 const reverse_iterator& rhs)
		{ return lhs.current == rhs.current; }

friend	bool operator!=	(const reverse_iterator& lhs,
						 const reverse_iterator& rhs)
		{ return lhs.current != rhs.current; }

friend	bool operator<	(const reverse_iterator& lhs,
						 const reverse_iterator& rhs)
		{ return lhs.current > rhs.current; }

friend	bool operator<=	(const reverse_iterator& lhs,
						 const reverse_iterator& rhs)
		{ return lhs.current >= rhs.current; }

friend	bool operator>	(const reverse_iterator& lhs,
						 const reverse_iterator& rhs)
		{ return lhs.current < rhs.current; }

friend	bool operator>=	(const reverse_iterator& lhs,
						 const reverse_iterator& rhs)
		{ return lhs.current <= rhs.current; }

friend	reverse_iterator	operator+(difference_type lhs, reverse_iterator rhs)
		{ return rhs += lhs; }

friend	difference_type		operator-(const reverse_iterator& lhs, const reverse_iterator& rhs)
		{ return lhs._ptr - rhs._ptr; }

	protected:
		iterator_type current;
	};

}

#endif
