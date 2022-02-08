#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
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
		typedef Key 						key_type;
		typedef Key 						value_type;
		typedef std::size_t 				size_type;
		typedef std::ptrdiff_t 				difference_type;
		typedef Compare 					key_compare;
		typedef Compare 					value_compare;
		typedef Allocator 					allocator_type;
		typedef value_type& 				reference;
		typedef const value_type& 			const_reference;
		typedef Allocator::pointer 			pointer;
		typedef Allocator::const_pointer	const_pointer;

	private:
		struct _node
		{
			bool	is_red;
			_node*	parent;
			_node*	left;
			_node*	right;


			static _node empty(false, nullptr, nullptr, nullptr);
		};

		class value_compare
		{


		};

		template<bool IsConst>
		class _common_iterator : public std::iterator<
				std::bidirectional_iterator_tag,
				value_type,
				std::ptrdiff_t,
				typename ft::conditional<IsConst, const_pointer, pointer>::type,
				typename ft::conditional<IsConst, const_reference, reference>::type>
		{};

	public:
		typedef _common_iterator<false>					iterator;
		typedef	_common_iterator<false> 				const_iterator;
		typedef	ft::reverse_iterator<iterator> 			reverse_iterator;
		typedef	ft::reverse_iterator<const_iterator> 	const_reverse_iterator;

	};
}

#endif
