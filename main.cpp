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
//// 	#include <map.hpp>
//// 	#include <stack.hpp>
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
//// template<typename T>
//// class MutantStack : public ft::stack<T>
//// {
//// public:
//// 	MutantStack() {}
//// 	MutantStack(const MutantStack<T>& src) { *this = src; }
//// 	MutantStack<T>& operator=(const MutantStack<T>& rhs)
//// 	{
//// 		this->c = rhs.c;
//// 		return *this;
//// 	}
//// 	~MutantStack() {}
////
//// 	typedef typename ft::stack<T>::container_type::iterator iterator;
////
//// 	iterator begin() { return this->c.begin(); }
//// 	iterator end() { return this->c.end(); }
//// };
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
//// 	ft::stack<int> stack_int;
// 	ft::vector<Buffer> vector_buffer;
//// 	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
//// 	ft::map<int, int> map_int;
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
// 	return (0);
// }

#include "vector.hpp"
#include "stack.hpp"
#include "map.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <map>

//template<typename Integral>
//void f(Integral val, typename ft::enable_if<ft::is_integral<Integral>::value>::type* = 0)
//{
//	(void)val;
//	std::cout << 1 << std::endl;
//};
//
//template<class T>
//void f(T val, typename ft::enable_if<!ft::is_integral<T>::value>::type* = 0)
//{
//	(void)val;
//	std::cout << 2 << std::endl;
//}
//
//struct A
//{
//	size_t i;
//
//	A(size_t i): i(i) {}
//	A(const A& src): i(src.i)
//	{
//		if (i == 10)
//			throw std::exception();
//	}
//};

//struct B
//{
//	~B()
//	{ std::cout << "Destuctor" << std::endl; }
//};

//struct node
//{
//	bool		is_red;
//	node*		parent;
//	node*		left;
//	node*		right;
//	std::string key;
//
//	node(): is_red(false), parent(nullptr), left(nullptr), right(nullptr), key(std::string()) {};
//
//	bool is_nil(const _node& node)
//	{ return !(node.is_red || node.parent || node.left || node.right || node.right); }
//};
//
//struct tree
//{
//	node	*root;
//
//	tree(): root(new node());
//
//	node*	find(const std::string& key)
//	{
//		node* tmp = root;
//		while (!tmp->is_nil())
//		{
//			if (key == tmp->key)
//				return &tmp;
//			if (key < tmp->key)
//				tmp = tmp->left;
//			else
//				tmp = tmp->right;
//		}
//		return tmp;
//	}
//
//	void insert(std::string key)
//	{
//		node* place = find(key);
//		if (!place->is_nil())
//			return;
//
//
//
//	}
//
//
//};

int	main()
{
//	node root;
//	root.is_red = false;
	
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
	ft::map<int, int> map;
	for (int i = 0; i < 7; ++i)
	{
		int num = rand() % 100;
		map[num] = num;
	}
    map.erase(73);
	ft::map<int, int> map2 = map;
	ft::map<int, int>::iterator it = map2.begin();
    for (; it != map2.end(); ++it)
        std::cout << it->first << ' ' << it->second << std::endl;
    std::cout << std::endl;
	--it;
	for (; it != map2.begin(); --it)
		std::cout << it->first << ' ' << it->second << std::endl;
	std::cout << it->first << ' ' << it->second << std::endl;
	std::cout << std::endl;
	for (; it != map2.end(); ++it)
		std::cout << it->first << ' ' << it->second << std::endl;
    std::cout << std::endl;
    for (ft::map<int, int>::reverse_iterator it1 = map2.rbegin(); it1 != map2.rend(); ++it1)
        std::cout << it1->first << ' ' << it1->second << std::endl;
	return 0;
}

//findNumbers() {
//	cv::UMat sourseImage;
//	cv::UMat gray; cv::cvtColor(sourseImage, gray, CV_BGR2GRAY);
//	equalizeHist(gray, gray);
//	bilateralFilter(gray, gray, 3, 45, 45);
//	std::vector<cv::Rect> plates;
//	//Нахождение областей номеров с помощью каскада Хаара
//	 cascadePlate.detectMultiScale(gray, plates,1.1,10,CV_HAAR_DO_CANNY_PRUN ING,cv::Size(50, 15), cv::Size(500, 150));
//	for(auto& p : plates) {
////выделение части изображения с номером
//		findLetters(imagePart);
//	}
//}
//findLetters(imagePart)
//{
////применение фильтров для улучшения качества изображения
//		cv::UMat cannyOutput, smothed, srcThreshold;
//		auto clahe = cv::createCLAHE(2.0, cv::Size(8, 8)); clahe->apply(imagePart, imagePart);
//		threshold(imagePart,srcThreshold,0,255,CV_THRESH_BINARY,CV_THRESH_OTSU);
//		medianBlur(srcThreshold, srcThreshold, 3);
//		cv::blur(srcThreshold, srcThreshold, cv::Size(3,3));
//		cv::Canny(srcThreshold, cannyOutput, 66, 133, 3);
//		std::vector<std::vector<cv::Point>> rectContours;
//		cv::findContours(cannyOutput, rectContours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE); //сортировка контуров по размеру
//		std::sort(rectContours.begin(),
//		std::vector<cv::Point> &second)
//		{
//			return cv::contourArea(first) >
//				   cv::contourArea(second); });
//// вычисление угла angle на который повернута область номера
//		(rectContours[0])
//		...
//		rotateImage(srcThreshold, angle); cv::Canny(srcThreshold, cannyOutput, 66,
//		133, 3); cv::findContours(cannyOutput,
//		rectContours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//		for(auto& c : contours)
//		{
////если размер области соответствует
//			размеру номера, то применить распознавание символа в области c помощью TesseractOCR recognizeChar(charPart);
//		}}
//rectContours.end(),
//[](std::vector<cv::Point>
//		&first,
