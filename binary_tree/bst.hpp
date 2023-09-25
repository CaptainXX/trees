#ifndef BST_HPP
#define BST_HPP

#include <iostream>

namespace ds_practice {

#define CREATE_OPERATORS_FOR_TYPE(TYPE) \
    inline bool operator<(const TYPE& rhs) { return this->data_ < rhs.data_; } \
    inline bool operator>(const TYPE& rhs) { return this->data_ > rhs.data_; } \
    inline bool operator==(const TYPE& rhs) { return this->data_ == rhs.data_; } \
    inline bool operator<(const T& rhs) { return this->data_ < rhs; } \
    inline bool operator>(const T& rhs) { return this->data_ > rhs; } \
    inline bool operator==(const T& rhs) { return this->data_ == rhs; } \
    friend std::ostream& operator<<(std::ostream& os, const TYPE& node) { \
        os << node.data_; \
        return os; \
    }

template<typename T>
struct TreeNodeBase {
    T data_;
    TreeNodeBase *left_;
    TreeNodeBase *right_;
    TreeNodeBase *parent_;

    TreeNodeBase(): left_(nullptr),
               right_(nullptr),
               parent_(nullptr) {}

    TreeNodeBase(T data): data_(data),
                     left_(nullptr),
                     right_(nullptr),
                     parent_(nullptr) {}

    CREATE_OPERATORS_FOR_TYPE(TreeNodeBase);
};

template<typename T, typename TreeNode = TreeNodeBase<T>>
class BinaryTreeBase {
 public:
    using TreeNodeType = TreeNode;

    BinaryTreeBase():root_(nullptr) {}

    BinaryTreeBase(const BinaryTreeBase&) = delete;
    BinaryTreeBase& operator=(const BinaryTreeBase&) = delete;

    virtual ~BinaryTreeBase() {
        Destroy(root_);
    }

    TreeNode* Insert(const T& data);
    TreeNode* Search(const T& target);
    bool Delete(const T& target);

    template<typename U, typename TreeNodeT>
    friend std::ostream& operator<<(std::ostream& os, const BinaryTreeBase<U, TreeNodeT>& bst);

 protected:
    TreeNode *root_;

    void Destroy(TreeNode* node);
    void InorderPrint(std::ostream& os, TreeNode* node) const;

    virtual bool InsertRecursively(TreeNode*& parent, TreeNode* node) = 0;
    virtual TreeNode* SearchRecursively(TreeNode* node, const T& target) = 0;
    virtual bool DeleteRecursively(TreeNode* node, const T& target) = 0;
}; // class BinaryTreeBase

template<typename T, typename TreeNode>
TreeNode* BinaryTreeBase<T, TreeNode>::Insert(const T& data) {
    TreeNode *node_to_insert = new TreeNode(data);
    if (InsertRecursively(root_, node_to_insert))
        return node_to_insert;

    delete node_to_insert;
    return nullptr;
}

template<typename T, typename TreeNode>
TreeNode* BinaryTreeBase<T, TreeNode>::Search(const T& target) {
    return SearchRecursively(root_, target);
}

template<typename T, typename TreeNode>
bool BinaryTreeBase<T, TreeNode>::Delete(const T& target) {
    return DeleteRecursively(root_, target);
}

template<typename T, typename TreeNode>
std::ostream& operator<<(std::ostream& os, const BinaryTreeBase<T, TreeNode>& bst) {
    os << "[ ";
    bst.InorderPrint(os, bst.root_);
    os << "]\n";
    return os;
}

template<typename T, typename TreeNode>
void BinaryTreeBase<T, TreeNode>::Destroy(TreeNode* node) {
    if (!node) return;

    if (node->left_) Destroy(node->left_);
    if (node->right_) Destroy(node->right_);
    delete node;
}

template<typename T, typename TreeNode>
void BinaryTreeBase<T, TreeNode>::InorderPrint(std::ostream& os, TreeNode* node) const {
    if (!node) return;
    if (node->left_) InorderPrint(os, node->left_);
    os << *node << " ";
    if (node->right_) InorderPrint(os, node->right_);
}

// ------------ Binary Search Tree -------------

template<typename T>
class BinarySearchTree final : public BinaryTreeBase<T> {
 public:
    BinarySearchTree() {}
    ~BinarySearchTree() {}

 protected:
    using TreeNode = typename BinaryTreeBase<T>::TreeNodeType;

    bool InsertRecursively(TreeNode*& parent, TreeNode* node) override;
    TreeNode* SearchRecursively(TreeNode* node, const T& target) override;
    bool DeleteRecursively(TreeNode* node, const T& target) override;

};

template<typename T>
bool BinarySearchTree<T>::InsertRecursively(TreeNode*& parent, TreeNode* node) {
    if (!parent) {
        parent = node;
        return true;
    }

    if (*node < *parent) {
        if (parent->left_) {
            return InsertRecursively(parent->left_, node);
        }

        node->parent_ = parent;
        parent->left_ = node;
        return true;
    } else if (*node > *parent) {
        if (parent->right_) {
            return InsertRecursively(parent->right_, node);
        }

        node->parent_ = parent;
        parent->right_ = node;
        return true;
    }
    return false;
}

template<typename T>
typename BinarySearchTree<T>::TreeNode*
BinarySearchTree<T>::SearchRecursively(TreeNode* node, const T& target) {
    if (!node) return nullptr;
    if (*node == target) return node;
    else if (*node > target) {
        if (node->left_ && SearchRecursively(node->left_, target)) return node->left_;
    } else {
        if (node->right_ && SearchRecursively(node->right_, target)) return node->right_;
    }

    return nullptr;
}

template<typename T>
bool BinarySearchTree<T>::DeleteRecursively(TreeNode* node, const T& target) {
    if (!node) return false;

    if (*node > target) {
        return DeleteRecursively(node->left_, target);
    } else if (*node < target) {
        return DeleteRecursively(node->right_, target);
    } else {
        TreeNode *parent = node->parent_;
        bool is_root = (parent == nullptr);
        bool is_left_child = (!is_root && (parent->left_ == node));

        if (!node->left_ && !node->right_) {
            delete node;
            if (is_root) BinaryTreeBase<T>::root_ = nullptr;
            else {
                if (is_left_child) parent->left_ = nullptr;
                else parent->right_ = nullptr;
            }

        } else if (!node->right_) {
            TreeNode *l_node = node->left_;
            if (is_root) {
                BinaryTreeBase<T>::root_ = l_node;
            } else {
                if (is_left_child) parent->left_ = l_node;
                else parent->right_ = l_node;
            }
            l_node->parent_ = parent;
            delete node;

        } else if (!node->left_) {
            TreeNode *r_node = node->right_;
            if (is_root) {
                BinaryTreeBase<T>::root_ = r_node;
            } else {
                if (is_left_child) parent->left_ = r_node;
                else parent->right_ = r_node;
            }
            r_node->parent_ = parent;
            delete node;

        } else {
            TreeNode *ino_prev = node->left_;
            while (ino_prev->right_) {
                ino_prev = ino_prev->right_;
            }
            node->data_ = ino_prev->data_;

            return DeleteRecursively(ino_prev, ino_prev->data_);
        }

        return true;
    }
}

// ------------ Red Black Tree -------------

template<typename T>
struct RBTreeNode {
    enum class Color {
        Red = 0,
        Black,
    };

    Color color_;
    T data_;
    RBTreeNode *left_;
    RBTreeNode *right_;
    RBTreeNode *parent_;

    RBTreeNode(): color_(Color::Red), data_(),
                  left_(nullptr), right_(nullptr),
                  parent_(nullptr) {}
    RBTreeNode(T data): color_(Color::Red), data_(data),
                        left_(nullptr), right_(nullptr),
                        parent_(nullptr) {}
    RBTreeNode(T data, Color color): color_(color), data_(data),
                        left_(nullptr), right_(nullptr),
                        parent_(nullptr) {}

    CREATE_OPERATORS_FOR_TYPE(RBTreeNode);
};

template<typename T>
class RBTree final : public BinaryTreeBase<T, RBTreeNode<T>> {
 public:
    RBTree() {}
    ~RBTree() {}

 protected:
    using TreeNode = RBTreeNode<T>;

    bool InsertRecursively(TreeNode*& parent, TreeNode* node) override;
    TreeNode* SearchRecursively(TreeNode* node, const T& target) override;
    bool DeleteRecursively(TreeNode* node, const T& target) override;

};

template<typename T>
bool RBTree<T>::InsertRecursively(TreeNode*& parent, TreeNode* node) {
    return false;
}

template<typename T>
typename RBTree<T>::TreeNode*
RBTree<T>::SearchRecursively(TreeNode* node, const T& target) {
    return nullptr;
}

template<typename T>
bool RBTree<T>::DeleteRecursively(TreeNode* node, const T& target) {
    return false;
}

// ------------ AVL Tree -------------

template<typename T>
class AVLTree final : public BinaryTreeBase<T> {
 public:
    AVLTree() {}
    ~AVLTree() {}

 protected:
    using TreeNode = typename BinaryTreeBase<T>::TreeNodeType;

    bool InsertRecursively(TreeNode*& parent, TreeNode* node) override;
    TreeNode* SearchRecursively(TreeNode* node, const T& target) override;
    bool DeleteRecursively(TreeNode* node, const T& target) override;

};

template<typename T>
bool AVLTree<T>::InsertRecursively(TreeNode*& parent, TreeNode* node) {
    return false;
}

template<typename T>
typename AVLTree<T>::TreeNode*
AVLTree<T>::SearchRecursively(TreeNode* node, const T& target) {
    return nullptr;
}

template<typename T>
bool AVLTree<T>::DeleteRecursively(TreeNode* node, const T& target) {
    return false;
}

}  // namespace bst

#endif