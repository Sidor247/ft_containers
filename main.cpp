/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwhis <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 16:07:59 by cwhis             #+#    #+#             */
/*   Updated: 2022/01/05 00:11:37 by cwhis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <iostream>
// #include <string>
// #include <deque>
// #if 0 //CREATE A REAL STL EXAMPLE
// 	#include <map>
// 	#include <stack>
// 	#include <vector>
// 	namespace ft = std;
// #else
// 	#include <map.hpp>
// 	#include <stack.hpp>
// 	#include <vector.hpp>
// #endif
//
// #include <stdlib.h>
//
// #define MAX_RAM 4294967296
// #define BUFFER_SIZE 4096
// struct Buffer
// {
// 	int idx;
// 	char buff[BUFFER_SIZE];
// };
//
//
// #define COUNT (MAX_RAM / (int)sizeof(Buffer))
//
// template<typename T>
// class MutantStack : public ft::stack<T>
// {
// public:
// 	MutantStack() {}
// 	MutantStack(const MutantStack<T>& src) { *this = src; }
// 	MutantStack<T>& operator=(const MutantStack<T>& rhs)
// 	{
// 		this->c = rhs.c;
// 		return *this;
// 	}
// 	~MutantStack() {}
//
// 	typedef typename ft::stack<T>::container_type::iterator iterator;
//
// 	iterator begin() { return this->c.begin(); }
// 	iterator end() { return this->c.end(); }
// };
//
// int main(int argc, char** argv) {
// 	if (argc != 2)
// 	{
// 		std::cerr << "Usage: ./test seed" << std::endl;
// 		std::cerr << "Provide a seed please" << std::endl;
// 		std::cerr << "Count value:" << COUNT << std::endl;
// 		return 1;
// 	}
// 	const int seed = atoi(argv[1]);
// 	srand(seed);
//
// 	ft::vector<std::string> vector_str;
// 	ft::vector<int> vector_int;
// 	ft::stack<int> stack_int;
// 	ft::vector<Buffer> vector_buffer;
// 	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
// 	ft::map<int, int> map_int;
//
// 	for (int i = 0; i < COUNT; i++)
// 	{
// 		vector_buffer.push_back(Buffer());
// 	}
//
// 	for (int i = 0; i < COUNT; i++)
// 	{
// 		const int idx = rand() % COUNT;
// 		vector_buffer[idx].idx = 5;
// 	}
// 	ft::vector<Buffer>().swap(vector_buffer);
//
// 	try
// 	{
// 		for (int i = 0; i < COUNT; i++)
// 		{
// 			const int idx = rand() % COUNT;
// 			vector_buffer.at(idx);
// 			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
// 		}
// 	}
// 	catch(const std::exception& e)
// 	{
// 		//NORMAL ! :P
// 	}
//
// 	for (int i = 0; i < COUNT; ++i)
// 	{
// 		map_int.insert(ft::make_pair(rand(), rand()));
// 	}
//
// 	int sum = 0;
// 	for (int i = 0; i < 10000; i++)
// 	{
// 		int access = rand();
// 		sum += map_int[access];
// 	}
// 	std::cout << "should be constant with the same seed: " << sum << std::endl;
//
// 	{
// 		ft::map<int, int> copy = map_int;
// 	}
// 	MutantStack<char> iterable_stack;
// 	for (char letter = 'a'; letter <= 'z'; letter++)
// 		iterable_stack.push(letter);
// 	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
// 	{
// 		std::cout << *it;
// 	}
// 	std::cout << std::endl;
//    ft::vector<int> v1;
//    ft::vector<int> v2;
//    std::swap(v1, v2);
// 	return (0);
// }
//
#include "vector.hpp"
#include "stack.hpp"
#include "map.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <map>

template <class T, class V>
std::vector<int> comparator_test(ft::map<T, V> mp) {
    std::vector<int> v;
    fillMap(mp);
//    for (typename ft::map<T, V>::iterator it = mp.begin(); it != mp.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp.end(); it != mp.begin(); it--) { v.push_back(it->first); }
//    ft::map<int, int, std::greater<int> > mp1;
//    fillMap(mp1);
//    v.push_back(mp1.begin()->first);
//    mp1.erase(41);
//    v.push_back(mp1.begin()->first);
//    mp1.erase(29);
//    v.push_back(mp1.begin()->first);
//    ft::map<int, int, std::greater<int> > mp2;
//    mp2.insert(ft::make_pair(3, 3));
//    v.push_back(mp2.begin()->first);
//    mp2.erase(3);
//    if (mp2.begin() == mp2.end())
//        v.push_back(1);
//    ft::map<int, int, std::plus<int> > mp3;
//    fillMap(mp3);
//    for (typename ft::map<T, V>::iterator it = mp3.begin(); it != mp3.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp3.end(); it != mp3.begin(); it--) { v.push_back(it->first); }
//    ft::map<int, int, std::minus<int> > mp4;
//    fillMap(mp4);
//    for (typename ft::map<T, V>::iterator it = mp4.begin(); it != mp4.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp4.end(); it != mp4.begin(); it--) { v.push_back(it->first); }
//    ft::map<int, int, std::greater_equal<int> > mp5;
//    fillMap(mp5);
//    for (typename ft::map<T, V>::iterator it = mp5.begin(); it != mp5.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp5.end(); it != mp5.begin(); it--) { v.push_back(it->first); }
//    ft::map<int, int, std::multiplies<int> > mp6;
//    fillMap(mp6);
//    for (typename ft::map<T, V>::iterator it = mp6.begin(); it != mp6.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp6.end(); it != mp6.begin(); it--) { v.push_back(it->first); }
//    ft::map<int, int, std::bit_xor<int> > mp7;
//    fillMap(mp7);
//    for (typename ft::map<T, V>::iterator it = mp7.begin(); it != mp7.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp7.end(); it != mp7.begin(); it--) { v.push_back(it->first); }
//    ft::map<int, int, std::logical_and<int> > mp8;
//    fillMap(mp8);
//    for (typename ft::map<T, V>::iterator it = mp8.begin(); it != mp8.end(); it++) { v.push_back(it->first); }
//    for (typename ft::map<T, V>::iterator it = --mp8.end(); it != mp8.begin(); it--) { v.push_back(it->first); }
//    v.push_back(mp1.size());

    return v;
}

class B {
public:
	char *l;
	int i;
	B():l(nullptr), i(1) {};
	B(const int &ex) {
		this->i = ex;
		this->l = new char('a');
	};
	virtual ~B() {
		delete this->l;
		this->l = nullptr;
	};
};

class A : public B {
public:
	A():B(){};
	A(const B* ex){
		this->l = new char(*(ex->l));
		this->i = ex->i;
		if (ex->i == -1) throw "n";
	}
	~A() {
		delete this->l;
		this->l = nullptr;
	};
};

int _ratio = 1;

template <typename T>
std::vector<int> insert_test_1(std::vector<T> vector) {
	std::vector<int> v;
	vector.assign(2600 * _ratio, 1);
	v.push_back(*(vector.insert(vector.end() - 800 * _ratio, 44)));
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	std::unique_ptr<B> k2(new B(3));
	std::unique_ptr<B> k3(new B(4));
	std::unique_ptr<B> k4(new B(-1));
	std::vector<A> vv;
	std::vector<B*> v1;

	v1.push_back(&(*k2));
	v1.push_back(&(*k3));
	v1.push_back(&(*k4));
	try { vv.insert(vv.begin(), v1.begin(), v1.end()); }
	catch (...) {
		v.push_back(vv.size());
		v.push_back(vv.capacity());
	}
	return v;
}

template <typename T>
std::vector<int> insert_test_1(ft::vector<T> vector) {
	std::vector<int> v;
	vector.assign(2600 * _ratio, 1);
	v.push_back(*(vector.insert(vector.end() - 800 * _ratio, 44)));
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	std::unique_ptr<B> k2(new B(3));
	std::unique_ptr<B> k3(new B(4));
	std::unique_ptr<B> k4(new B(-1));
	ft::vector<A> vv;
	ft::vector<B*> v1;

	v1.push_back(&(*k2));
	v1.push_back(&(*k3));
	v1.push_back(&(*k4));
	try { vv.insert(vv.begin(), v1.begin(), v1.end()); }
	catch (...) {
		v.push_back(vv.size());
		v.push_back(vv.capacity());
	}
	return v;
}

template <typename T>
std::vector<int> assign_test(std::vector<T> vector) {
	std::vector<int> v;
	std::vector<int> tmp, tmp2;
	vector.assign(3, 3);
	tmp.assign(4000 * _ratio, 1);
	tmp2.assign(4 * _ratio, 1);
	vector.assign(tmp.begin(), tmp.end());
	v.push_back(vector[1]);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	vector.assign(tmp2.begin(), tmp2.end());
	v.push_back(vector[1]);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	return v;
}

template <typename T>
std::vector<int> assign_test(ft::vector<T> vector) {
	std::vector<int> v;
	ft::vector<int> tmp, tmp2;
	vector.assign(3, 3);
	tmp.assign(4000 * _ratio, 1);
	tmp2.assign(4 * _ratio, 1);
	vector.assign(tmp.begin(), tmp.end());
	v.push_back(vector[1]);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	vector.assign(tmp2.begin(), tmp2.end());
	v.push_back(vector[1]);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	return v;
}

template <typename T>
std::vector<int> insert_test_3(std::vector<T> vector) {
	std::vector<int> v;
	std::vector<int> tmp;
	tmp.assign(2600 * _ratio, 1);
	vector.assign(4200 * _ratio, 1);
	std::cout << "\t\t" << vector.capacity() << std::endl;
	vector.insert(vector.end() - 1000 * _ratio, tmp.begin(), tmp.end());
	v.push_back(vector[3]);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	return v;
}

template <typename T>
std::vector<int> insert_test_3(ft::vector<T> vector) {
	std::vector<int> v;
	ft::vector<int> tmp;
	tmp.assign(2600 * _ratio, 1);
	vector.assign(4200 * _ratio, 1);
	vector.insert(vector.end() - 1000 * _ratio, tmp.begin(), tmp.end());
	v.push_back(vector[3]);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	return v;
}

template <typename T>
std::vector<int> resize_test(std::vector<T> vector) {
	std::vector<int> v;
	vector.assign(9900 * _ratio, 1);
	vector.resize(5000 * _ratio);
	vector.reserve(5000 * _ratio);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	vector.resize(7000 * _ratio);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	vector.resize(15300 * _ratio, T());
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	v.push_back(vector[65]);
	return v;
}

template <typename T>
std::vector<int> resize_test(ft::vector<T> vector) {
	std::vector<int> v;
	vector.assign(9900 * _ratio, 1);
	vector.resize(5000 * _ratio);
	vector.reserve(5000 * _ratio);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	vector.resize(7000 * _ratio);
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	vector.resize(15300 * _ratio, T());
	v.push_back(vector.size());
	v.push_back(vector.capacity());
	v.push_back(vector[65]);
	return v;
}

int	main()
{
//    mp3.insert(ft::make_pair(7, 3));
//    mp3.insert(ft::make_pair(19, 3));
//    mp3.insert(ft::make_pair(29, 3));
//    mp3.insert(ft::make_pair(41, 3));
//    mp3.insert(ft::make_pair(4, 3));
//    mp3.insert(ft::make_pair(11, 3));
//    for (ft::map<int, int, std::plus<int> >::iterator it = mp3.begin(); it != mp3.end(); it++) { v.push_back(it->first); }
//    for (ft::map<int, int, std::plus<int> >::iterator it = --mp3.end(); it != mp3.begin(); it--) { v.push_back(it->first); }
	// ft::vector<int> v1(3u, 4);
	// ft::vector<int> v2(2u, 7);
	// std::cout << v1.size() << std::endl;
	// std::cout << v2.size() << std::endl;
	// for (ft::vector<int>::iterator i = v1.begin(); i != v1.end(); ++i)
	// 	std::cout << *i << std::endl;
	// for (ft::vector<int>::iterator i = v2.begin(); i != v2.end(); ++i)
	// 	std::cout << *i << std::endl;
	// v1.swap(v2);
	// std::cout << std::endl;
	// for (ft::vector<int>::iterator i = v1.begin(); i != v1.end(); ++i)
	// 	std::cout << *i << std::endl;
	// for (ft::vector<int>::iterator i = v2.begin(); i != v2.end(); ++i)
	// 	std::cout << *i << std::endl;
	// ft::vector<int> v(0, 3);
	// std::cout << v.capacity() << std::endl;
	// std::cout << v.size() << std::endl;
	// std::cout << v.data() << std::endl;
	// v.push_back(4);
	// v.push_back(4);
	// v.push_back(4);
	// v.push_back(4);
	// v.push_back(4);
	// v.push_back(4);
	// for (ft::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	// 	std::cout << *it << std::endl;
//	ft::vector<int> v1;
//	for (int i = 0; i < 10; ++i)
//		v1.push_back(i);
//	for (ft::vector<int>::iterator it = v1.begin(); it != v1.end(); ++it)
//		std::cout << *it << std::endl;
//	std::cout << std::endl;
//	ft::vector<int> v(v1.begin(), v1.end() - 3);
//
//	int	arr[5] = {0, 1, 2, 3, 4};
//	int *p = arr;
//	ft::vector<int> l;
//	l.push_back(1);
//	l.push_back(2);
//	l.push_back(3);
//	l.push_back(4);
//	l.push_back(5);
//	l.push_back(6);
//	ft::vector<double>	v(p, p + 5);
//	for (ft::vector<double>::iterator it = v.begin(); it != v.end(); ++it)
//		std::cout << *it << std::endl;
//	std::cout << "reversed" << std::endl;
//	for (ft::vector<double>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it)
//		std::cout << *it << std::endl;
//	std::swap(v, v);
//	ft::vector<int>	v;
//	v.insert(v.begin(), 1);
//	v.reserve(0);
//	std::cout << v.data() << std::endl << *(v.data()) << std::endl;
//	v.push_back(2);
//	v.push_back(4);
//	v.insert(v.begin() + 2, 3);
//	v.insert(v.end(), 5);
//	ft::vector<int>::iterator it = v.insert(v.begin(), 0);
//	for (; it != v.end(); ++it)
//		std::cout << *it << std::endl;
//	for (size_t i = 0; i < 15; ++i)
//		v.push_back(A(i));
//	try
//	{ v.insert(v.end(), A(4)); }
//	catch (std::exception&)
//	{ std::cout << "exception" << std::endl; }
//	std::cout << v.size() << ' ' << v.capacity() << std::endl;
//	ft::vector<int>	v1;
//	v1.insert(v1.end(), 10, 10);
//	std::cout << v1.capacity() << ' ' << v1.size() << std::endl;
//	for (ft::vector<int>::iterator it = v1.begin(); it != v1.end(); ++it)
//		std::cout << *it << std::endl;
//
//	ft::vector<int> v2;
//	v2.insert(v2.begin(), v1.begin(), v1.end());
//	std::cout << v2.capacity() << ' ' << v2.size() << std::endl;
//	for (ft::vector<int>::iterator it = v2.begin(); it != v2.end(); ++it)
//		std::cout << *it << std::endl;
//	std::vector<int>	v;
//	v.erase(v.begin());
//	std::vector<int>	v;
//	for (int i = 0; i < 10; ++i)
//		v.push_back(i);
//	v.erase(v.begin() + 3, v.end() - 4);
//	std::cout << v.size() << v.capacity() << std::endl;
//	for (int i = 0; i < 10; ++i)
//		v.push_back(i);
//
//	ft::vector<int>::iterator pos = v.erase(v.begin() + 4);
//	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//		std::cout << *it << std::endl;
//	std::cout << *pos << std::endl;

//    ft::vector<ft::pair<int, int> > v;
//    v.push_back(ft::pair<int, int>(1, 1));
//    for (ft::vector<ft::pair<int, int> >::reverse_iterator it = v.rbegin(); it != v.rend(); ++it)
//        std::cout << it->first << std::endl;
//    ft::vector<std::pair<int,int> > v;
//    std::map<int, int> map;
//	for (int i = 0; i < 10; ++i)
//	{
//		int num = rand() % 100;
//		v.push_back(std::make_pair(num, num));
//	}
//    map.insert(v.begin(), v.end());
//    map.erase(7);
//    std::map<int, int> map2 = map;
//    std::swap(map, map2);
//    std::map<int, int>::iterator it = map2.begin();
//    it = map2.upper_bound(3);
//    it = map2.begin();
//    for (; it != map2.end(); ++it)
//        std::cout << it->first << ' ' << it->second << std::endl;
//    std::cout << std::endl;
//	--it;
//	for (; it != map2.begin(); --it)
//		std::cout << it->first << ' ' << it->second << std::endl;
//	std::cout << it->first << ' ' << it->second << std::endl;
//	std::cout << std::endl;
//	for (; it != map2.end(); ++it)
//		std::cout << it->first << ' ' << it->second << std::endl;
//    std::cout << std::endl;
//    std::map<int, int>::const_reverse_iterator it1 = map2.rbegin();
//    for (; it1 != map2.rend(); ++it1)
//        std::cout << it1->first << ' ' << it1->second << std::endl;
//    std::cout << std::endl;
//    --it1;
//    for (; it1 != map2.rbegin(); --it)
//        std::cout << it1->first << ' ' << it1->second << std::endl;
//    std::cout << it1->first << ' ' << it1->second << std::endl;
//    ft::map<int, int> mp;
//    std::vector<int> v;
//    map.erase(1);
//    for (int i = 0; i < 30; ++i)
//        map.insert(ft::make_pair(i, i));
//    for (ft::map<int, int>::iterator it = map.begin(); it != map.end(); it = map.begin())
//        map.erase(it->first);
//    map.erase(1);
//    for (int i = 0; i < 30; ++i)
//        map.insert(ft::make_pair(i, i));
    std::vector<int> v, v1;
	ft::vector<int> v2;
	v = resize_test(v1);
	for (size_t i = 0; i < v.size(); ++i)
		std::cout << v[i] << std::endl;
	std::cout << std::endl;
	v = resize_test(v2);
	for (size_t i = 0; i < v.size(); ++i)
		std::cout << v[i] << std::endl;
//	vector.assign(2600 * _ratio, 1);
//	std::cout << *vector.insert(vector.end() - 800 * _ratio, 44) << std::endl;
//    std::cout << tmp2.size() << ' ' << tmp2.capacity() << std::endl;
//    vector.assign(tmp.begin(), tmp.end());
//    std::cout << vector[1] << ' ' << vector.size() << ' ' << vector.capacity() << std::endl;
//    vector.assign(tmp2.begin(), tmp2.end());
//    std::cout << vector.size() << ' ' << vector.capacity() << std::endl;
	return 0;
}

