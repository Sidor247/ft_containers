#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft
{

template< class T1, class T2 >
struct pair
{
public:
	T1	first;
	T2	second;

	typedef T1 first_type;
	typedef T2 second_type;

	pair():
		first(T1()),
		second(T2()) {}

	pair( const T1& x, const T2& y ):
		first(x),
		second(y) {}

	template< class U1, class U2 >
	pair( const pair<U1, U2>& p ):
		first(p.first),
		second(p.second) {}

	pair( const pair& p ):
		first(p.first),
		second(p.second) {}

	pair& operator=( const pair& other )
	{
		if (this == &other)
			return *this;
		first = other.first;
		second = other.second;
		return *this;
	}
};

template< class T1, class T2 >
pair<T1,T2> make_pair( T1 t, T2 u )
{ return pair<T1, T2>(t, u); }

template< class T1, class T2 >
bool operator==( const pair<T1,T2>& lhs, const std::pair<T1,T2>& rhs );

template< class T1, class T2 >
bool operator!=( const pair<T1,T2>& lhs, const std::pair<T1,T2>& rhs );

template< class T1, class T2 >
bool operator<( const pair<T1,T2>& lhs, const std::pair<T1,T2>& rhs );

template< class T1, class T2 >
bool operator<=( const pair<T1,T2>& lhs, const std::pair<T1,T2>& rhs );

template< class T1, class T2 >
bool operator>( const pair<T1,T2>& lhs, const std::pair<T1,T2>& rhs );

template< class T1, class T2 >
bool operator>=( const pair<T1,T2>& lhs, const std::pair<T1,T2>& rhs );

}

#endif
