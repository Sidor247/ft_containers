#ifndef UTILS_HPP
#define UTILS_HPP

namespace ft
{
	template< class Iter >
	struct iterator_traits
	{
		typedef typename Iter::difference_type		difference_type;
		typedef typename Iter::value_type			value_type;
		typedef typename Iter::pointer				pointer;
		typedef typename Iter::reference			reference;
		typedef typename Iter::iterator_category	iterator_category;
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

	template<class T> struct is_pointer : false_type {};
	template<class T> struct is_pointer<T*> : true_type {};
	template<class T> struct is_pointer<T* const> : true_type {};
	template<class T> struct is_pointer<T* volatile> : true_type {};
	template<class T> struct is_pointer<T* const volatile> : true_type {};
}

#endif