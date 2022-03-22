#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <__nullptr>

template<typename T>
struct TreeNode
{
    typedef T value_type;
    
    bool		is_red;
    TreeNode*	parent;
    TreeNode*	left;
    TreeNode*	right;
    value_type* value;

    TreeNode():
		is_red(false),
		parent(nullptr),
		left(nullptr),
		right(nullptr),
		value(nullptr) {}

    static TreeNode* nil()
    {
        static TreeNode nil;
        return &nil;
    };

    bool is_nil()
    { return this == nil(); }

	TreeNode(TreeNode* root):
		is_red(false),
		parent(nullptr),
		left(root),
		right(root),
		value(nullptr) {}

    TreeNode(TreeNode* parent, bool is_red):
            is_red(is_red),
            parent(parent) {}

    TreeNode(const TreeNode& src):
            is_red(src.is_red),
            parent(src.parent),
            left(nil()),
            right(nil()),
            value(src.value) {}

    TreeNode*	advanced_left()
    {
        if (this == nil())
            return nil();
        TreeNode* ptr = this;
        while (!ptr->left->is_nil())
            ptr = ptr->left;
        return ptr;
    }

    TreeNode*	advanced_right()
    {
        if (this == nil())
            return nil();
        TreeNode* ptr = this;
        while (!ptr->right->is_nil())
            ptr = ptr->right;
        return ptr;
    }

    TreeNode*	grandparent() const
    { return parent->parent; }

    TreeNode*	uncle() const
    {
        TreeNode* g = grandparent();
        if (!g)
            return nil();
        if (parent == g->left)
            return g->right;
        else
            return g->left;
    }

    TreeNode*	sibling()
    {
        if (this == parent->left)
            return parent->right;
        else
            return parent->left;
    }

    TreeNode*& child_pointer()
    {
        if (this == parent->left)
            return parent->left;
        else
            return parent->right;
    }
};

#endif
