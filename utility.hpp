//
// Created by Colombo Whis on 3/12/22.
//

#ifndef FT_CONTAINERS_UTILITY_HPP
#define FT_CONTAINERS_UTILITY_HPP

#include <iterator>

namespace ft
{

    template<class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1,
               InputIt2 first2) {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (!(*first1 == *first2))
            { return false; }
        }
        return true;
    }

    template<class InputIt1, class InputIt2, class BinaryPredicate>
    bool equal(InputIt1 first1, InputIt1 last1,
               InputIt2 first2, BinaryPredicate p)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (!p(*first1, *first2))
            { return false; }
        }
        return true;
    }

    template<class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2) {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
        {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template<class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2,
                                 Compare comp) {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
        {
            if (comp(*first1, *first2)) return true;
            if (comp(*first2, *first1)) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template< class Iterator >
    struct iterator_traits
    {
        typedef typename Iterator::difference_type		difference_type;
        typedef typename Iterator::value_type			value_type;
        typedef typename Iterator::pointer				pointer;
        typedef typename Iterator::reference			reference;
        typedef typename Iterator::iterator_category	iterator_category;
    };

    template <class T>
    struct iterator_traits<T*>
    {
        typedef std::ptrdiff_t					difference_type;
        typedef T								value_type;
        typedef T*								pointer;
        typedef T&								reference;
        typedef std::random_access_iterator_tag	iterator_category;
    };

    template <class T>
    struct iterator_traits<const T*>
    {
        typedef std::ptrdiff_t					difference_type;
        typedef T								value_type;
        typedef const T*						pointer;
        typedef const T&						reference;
        typedef std::random_access_iterator_tag	iterator_category;
    };

    template<bool B, class T = void>
    struct enable_if {};
    template<class T>
    struct enable_if<true, T> { typedef T type; };

    template<bool B, class T, class F>
    struct conditional { typedef T type; };
    template<class T, class F>
    struct conditional<false, T, F> { typedef F type; };

    template<class T, T v>
    struct integral_constant {
        typedef T                   value_type;
        typedef integral_constant   type;

        enum { value = v };
    };
    typedef integral_constant<bool, true>	true_type;
    typedef integral_constant<bool, false>	false_type;

    template<class T, class U>
    struct is_same : false_type {};
    template<class T>
    struct is_same<T, T> : true_type {};

    template< class T > struct remove_cv                   { typedef T type; };
    template< class T > struct remove_cv<const T>          { typedef T type; };
    template< class T > struct remove_cv<volatile T>       { typedef T type; };
    template< class T > struct remove_cv<const volatile T> { typedef T type; };

    template<typename> struct is_integral_base:		false_type {};
    template<> struct is_integral_base<bool>: 		true_type {};
    template<> struct is_integral_base<char>:		true_type {};
    template<> struct is_integral_base<char16_t>:	true_type {};
    template<> struct is_integral_base<char32_t>:	true_type {};
    template<> struct is_integral_base<wchar_t>:	true_type {};
    template<> struct is_integral_base<short>:		true_type {};
    template<> struct is_integral_base<int>:		true_type {};
    template<> struct is_integral_base<long>: 		true_type {};
    template<> struct is_integral_base<long long>:	true_type {};

    template<typename T>
    struct is_integral: is_integral_base<typename remove_cv<T>::type>::type {};

    template< class T1, class T2 >
    struct pair
    {
    public:
        T1	first;
        T2	second;

        typedef T1 first_type;
        typedef T2 second_type;

        pair():
                first(),
                second() {}

        pair( const T1& x, const T2& y ):
                first(x),
                second(y) {}

        template<class U, class V>
        pair(const pair<U,V>& pr):
                first(pr.first),
                second(pr.second) {}

        pair& operator=( const pair& other )
        {
            if (this == &other)
                return *this;
            first = other.first;
            second = other.second;
            return *this;
        }

        friend	bool operator==( const pair& lhs, const pair& rhs )
        { return lhs.first == rhs.first && lhs.second == rhs.second; }

        friend	bool operator!=( const pair& lhs, const pair& rhs )
        { return !(lhs == rhs); }

        friend	bool operator<( const pair& lhs, const pair& rhs )
        {
            if (lhs.first < rhs.first)
                return true;
            else if (lhs.first < rhs.first)
                return false;
            if (lhs.second < rhs.second)
                return true;
            else
                return false;
        }

        friend	bool operator<=( const pair& lhs, const pair& rhs )
        { return !(rhs < lhs); }

        friend	bool operator>( const pair& lhs, const pair& rhs )
        { return rhs < lhs; }

        friend	bool operator>=( const pair& lhs, const pair& rhs )
        { return !(lhs < rhs); }
    };

    template< class T1, class T2 >
    pair<T1,T2> make_pair( T1 t, T2 u )
    { return pair<T1, T2>(t, u); }

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

        reverse_iterator(): current(iterator_type()) {}
        explicit	reverse_iterator(iterator_type it): current(it) {}
        reverse_iterator(const reverse_iterator& rev_it): current(rev_it.current) {}

        iterator_type base() const
        { return current; }

        reference operator*() const
        { return current.operator*(); }

        pointer operator->() const
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
};

#endif //FT_CONTAINERS_UTILITY_HPP
