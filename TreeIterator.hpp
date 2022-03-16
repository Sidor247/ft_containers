#ifndef TREEITERATOR_HPP
#define TREEITERATOR_HPP

#include <iterator>
#include "utility.hpp"

namespace ft {

    template<typename Node, bool IsConst>
    class TreeIterator
    {
    protected:

    public:
        typedef typename ft::conditional<IsConst, const Node*, Node*>::type node_pointer;
        node_pointer _ptr;

        typedef std::bidirectional_iterator_tag                                         iterator_category;
        typedef typename Node::value_type                                               value_type;
        typedef std::ptrdiff_t                                                          difference_type;
        typedef typename ft::conditional<IsConst, const value_type*, value_type*>::type pointer;
        typedef typename ft::conditional<IsConst, const value_type&, value_type&>::type reference;

        TreeIterator() : _ptr(nullptr) {}

        TreeIterator(node_pointer ptr): _ptr(ptr) {}


        TreeIterator(const TreeIterator& src) : _ptr(src._ptr) {}

        TreeIterator &operator=(const TreeIterator& src)
        {
            if (this == &src)
                return *this;
            _ptr = src._ptr;
            return *this;
        }

        reference operator*() const
        { return *_ptr->value; }

        pointer operator->() const
        { return _ptr->value; }

        TreeIterator &operator++()
        {
            if (_ptr->right != Node::nil())
                _ptr = _ptr->right->advanced_left();
            else {
                node_pointer tmp = _ptr->parent;
                while (_ptr == tmp->right) {
                    _ptr = tmp;
                    tmp = tmp->parent;
                    if (!tmp)
                        return *this;
                }
                _ptr = tmp;
            }
            return *this;
        }

        TreeIterator operator++(int) {
            TreeIterator copy(*this);
            operator++();
            return copy;
        }

        TreeIterator &operator--() {
            if (_ptr->left != Node::nil())
                _ptr = _ptr->left->advanced_right();
            else {
                node_pointer tmp = _ptr->parent;
                while (_ptr == tmp->left) {
                    _ptr = tmp;
                    tmp = tmp->parent;
                    if (!tmp)
                        return *this;
                }
                _ptr = tmp;
            }
            return *this;
        }

        TreeIterator operator--(int) {
            TreeIterator copy(*this);
            operator--();
            return copy;
        }

        operator TreeIterator<Node, true>() const
        { return TreeIterator<Node, true>(_ptr); }

        template<typename N, bool B1, bool B2>
        friend bool operator==(const TreeIterator<N, B1>& lhs, const TreeIterator<N, B2>& rhs)
        { return lhs._ptr == rhs._ptr; }

        template<typename N, bool B1, bool B2>
        friend bool operator!=(const TreeIterator<N, B1>& lhs, const TreeIterator<N, B2>& rhs)
        { return lhs._ptr != rhs._ptr; }
    };
};

#endif
