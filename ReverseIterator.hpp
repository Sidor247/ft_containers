#ifndef REVERSEITERATOR_HPP
#define REVERSEITERATOR_HPP

#include "TypeTraits.hpp"

namespace ft
{
	template< class Iter >
	class reverse_iterator
	{
	public:
		typedef Iter 												iterator_type;
		typedef typename iterator_traits<Iter>::iterator_category	iterator_category;
		typedef	typename iterator_traits<Iter>::value_type			value_type;
		typedef	typename iterator_traits<Iter>::difference_type		difference_type;
		typedef	typename iterator_traits<Iter>::pointer 			pointer;
		typedef	typename iterator_traits<Iter>::reference 			reference;

		reverse_iterator():
			_iter(Iter(nullptr)) {}

		explicit	reverse_iterator(iterator_type it):
						_iter(it) {}

		reverse_iterator(const reverse_iterator& rev_it):
			_iter(rev_it._iter) {}

		iterator_type base() const
		{ return _iter; }

		reference operator*() const
		{ return _iter.operator*(); }

		reverse_iterator operator+(difference_type n) const
		{ return reverse_iterator(_iter - n); }

		reverse_iterator& operator++()
		{
			--_iter;
			return *this;
		}

		reverse_iterator  operator++(int)
		{
			reverse_iterator copy(_iter--);
			return copy;
		}

		reverse_iterator& operator+=(difference_type n)
		{
			_iter -= n;
			return *this;
		}

		reverse_iterator operator-(difference_type n) const
		{ return reverse_iterator(_iter + n); }

		reverse_iterator& operator--()
		{
			++_iter;
			return *this;
		}

		reverse_iterator  operator--(int)
		{
			reverse_iterator copy(_iter--);
			return copy;
		}

		reverse_iterator& operator-=(difference_type n)
		{
			_iter += n;
			return *this;
		}

		pointer operator->() const
		{ return _iter.operator->(); }

		reference operator[] (difference_type n) const
		{ return _iter[-n-1]; }

friend	reverse_iterator	operator+(difference_type lhs, const reverse_iterator& rhs)
		{ return rhs + lhs; }

friend	difference_type		operator-(const reverse_iterator& lhs, const reverse_iterator& rhs)
		{ return rhs - lhs; }

friend	bool operator==	(const reverse_iterator& lhs, const reverse_iterator& rhs);

friend	bool operator!=	(const reverse_iterator& lhs, const reverse_iterator& rhs)
		{ return lhs._iter != rhs._iter; }

friend	bool operator<	(const reverse_iterator& lhs, const reverse_iterator& rhs);

friend	bool operator<=	(const reverse_iterator& lhs, const reverse_iterator& rhs);

friend	bool operator>	(const reverse_iterator& lhs, const reverse_iterator& rhs);

friend	bool operator>=	(const reverse_iterator& lhs, const reverse_iterator& rhs);

	private:
		iterator_type _iter;
	};

}

#endif
