#ifndef BT_HPP
#define BT_HPP

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

namespace binary_tree {

#define CREATE_BASE_TREETYPE_MEMBERS(TYPE) \
    T data_; \
    TYPE *left_; \
    TYPE *right_; \
    TYPE *parent_; \

#define CREATE_OPERATORS_FOR_TYPE(TYPE) \
    inline bool operator<(const TYPE& rhs) const { return this->data_ < rhs.data_; } \
    inline bool operator<=(const TYPE& rhs) const { return this->data_ <= rhs.data_; } \
    inline bool operator>(const TYPE& rhs) const { return this->data_ > rhs.data_; } \
    inline bool operator>=(const TYPE& rhs) const { return this->data_ >= rhs.data_; } \
    inline bool operator==(const TYPE& rhs) const { return this->data_ == rhs.data_; } \
    inline bool operator<(const T& rhs) const { return this->data_ < rhs; } \
    inline bool operator<=(const T& rhs) const { return this->data_ <= rhs; } \
    inline bool operator>(const T& rhs) const { return this->data_ > rhs; } \
    inline bool operator>=(const T& rhs) const { return this->data_ >= rhs; } \
    inline bool operator==(const T& rhs) const { return this->data_ == rhs; } \
    \
    friend std::ostream& operator<<(std::ostream& os, const TYPE& node) { \
        os << node.ToString(); \
        return os; \
    }

template<typename T>
struct TreeNodeBase {
    CREATE_BASE_TREETYPE_MEMBERS(TreeNodeBase);

    TreeNodeBase(): left_(nullptr),
               right_(nullptr),
               parent_(nullptr) {}

    TreeNodeBase(T data): data_(data),
                     left_(nullptr),
                     right_(nullptr),
                     parent_(nullptr) {}

    inline std::string ToString() const {
        return std::to_string(data_);
    }

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
    TreeNode* Search(const T& target) const;
    bool Delete(const T& target);

    void Clear();
    int GetHeight() const;

    template<typename U, typename TreeNodeT>
    friend std::ostream& operator<<(std::ostream& os, const BinaryTreeBase<U, TreeNodeT>& bst);

 protected:
    TreeNode *root_;

    void Destroy(TreeNode* node);
    void InorderPrint(std::ostream& os, TreeNode* node) const;
    void TreePrint(std::ostream& os, TreeNode* node) const;
    void TreePrintInternal(std::ostream& os, TreeNode* node, const std::string& prefix, bool is_left) const;

    int GetHeightInternal(TreeNode* node) const;

    virtual bool InsertRecursively(TreeNode*& parent, TreeNode* node) = 0;
    virtual TreeNode* SearchRecursively(TreeNode* node, const T& target) const = 0;
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
TreeNode* BinaryTreeBase<T, TreeNode>::Search(const T& target) const {
    return SearchRecursively(root_, target);
}

template<typename T, typename TreeNode>
bool BinaryTreeBase<T, TreeNode>::Delete(const T& target) {
    return DeleteRecursively(root_, target);
}

template<typename T, typename TreeNode>
int BinaryTreeBase<T, TreeNode>::GetHeight() const {
    return GetHeightInternal(root_);
}

template<typename T, typename TreeNode>
void BinaryTreeBase<T, TreeNode>::Clear() {
    Destroy(root_);
    root_ = nullptr;
}

template<typename T, typename TreeNode>
int BinaryTreeBase<T, TreeNode>::GetHeightInternal(TreeNode* node) const {
    if (!node) {
        return 0;
    }

    int left_height = GetHeightInternal(node->left_);
    int right_height = GetHeightInternal(node->right_);

    return std::max(left_height, right_height) + 1;
}

template<typename T, typename TreeNode>
std::ostream& operator<<(std::ostream& os, const BinaryTreeBase<T, TreeNode>& bst) {
    os << "[ ";
    bst.InorderPrint(os, bst.root_);
    os << "]\n";
    bst.TreePrint(os, bst.root_);
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


template<typename T, typename TreeNode>
void BinaryTreeBase<T, TreeNode>::TreePrintInternal(std::ostream& os, TreeNode* node,
                                                    const std::string& prefix, bool is_left) const {
    os << prefix << (is_left ?  "├──" : "└──" );

    if (node) {
        os << *node << std::endl;

        TreePrintInternal(os, node->left_,  prefix + (is_left ? "│   " : "    "), true);
        TreePrintInternal(os, node->right_, prefix + (is_left ? "│   " : "    "), false);
    } else {
        os << "nil" << std::endl;
    }
}

template<typename T, typename TreeNode>
void BinaryTreeBase<T, TreeNode>::TreePrint(std::ostream& os, TreeNode* node) const {
    TreePrintInternal(os, node, "", false);
}

// ------------ Binary Search Tree -------------

template<typename T>
class BinarySearchTree final : public BinaryTreeBase<T> {
 public:
    BinarySearchTree() {}
    ~BinarySearchTree() {}

 protected:
    using TreeNode = typename BinaryTreeBase<T>::TreeNodeType;
    using BaseTreeType = BinaryTreeBase<T>;

    bool InsertRecursively(TreeNode*& parent, TreeNode* node) override;
    TreeNode* SearchRecursively(TreeNode* node, const T& target) const override;
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
BinarySearchTree<T>::SearchRecursively(TreeNode* node, const T& target) const {
    if (!node) return nullptr;
    if (*node == target) return node;
    else if (*node > target) {
        return SearchRecursively(node->left_, target);
    } else {
        return SearchRecursively(node->right_, target);
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
            if (is_root) BaseTreeType::root_ = nullptr;
            else {
                if (is_left_child) parent->left_ = nullptr;
                else parent->right_ = nullptr;
            }

        } else if (!node->right_) {
            TreeNode *l_node = node->left_;
            if (is_root) {
                BaseTreeType::root_ = l_node;
            } else {
                if (is_left_child) parent->left_ = l_node;
                else parent->right_ = l_node;
            }
            l_node->parent_ = parent;
            delete node;

        } else if (!node->left_) {
            TreeNode *r_node = node->right_;
            if (is_root) {
                BaseTreeType::root_ = r_node;
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

    CREATE_BASE_TREETYPE_MEMBERS(RBTreeNode);

    RBTreeNode(): color_(Color::Red), data_(),
                  left_(nullptr), right_(nullptr),
                  parent_(nullptr) {}
    RBTreeNode(T data): color_(Color::Red), data_(data),
                        left_(nullptr), right_(nullptr),
                        parent_(nullptr) {}
    RBTreeNode(T data, Color color): color_(color), data_(data),
                        left_(nullptr), right_(nullptr),
                        parent_(nullptr) {}

    inline bool IsRed() const { return color_ == Color::Red; }
    inline void SetRed() { color_ = Color::Red; }
    inline void SetBlack() { color_ = Color::Black; }
    inline std::string ToString() const {
        return std::to_string(data_) + " " + (color_ == Color::Red ? "R" : "B");
    }

    CREATE_OPERATORS_FOR_TYPE(RBTreeNode);
};

template<typename T>
class RBTree final : public BinaryTreeBase<T, RBTreeNode<T>> {
 public:
    RBTree() {}
    ~RBTree() {}

    void TestRotate(bool rot_left, RBTreeNode<T>* node);

 protected:
    using TreeNode = RBTreeNode<T>;
    using BaseTreeType = BinaryTreeBase<T, RBTreeNode<T>>;

    bool InsertRecursively(TreeNode*& parent, TreeNode* node) override;
    TreeNode* SearchRecursively(TreeNode* node, const T& target) const override;
    bool DeleteRecursively(TreeNode* node, const T& target) override;

    void LeftRotate(TreeNode* node);
    void RightRotate(TreeNode* node);

    void InsertFixUp(TreeNode*& node);

};

template<typename T>
void RBTree<T>::TestRotate(bool rot_left, RBTreeNode<T>* node) {
    if (rot_left) {
        LeftRotate(node);
    } else {
        RightRotate(node);
    }
}

template<typename T>
bool RBTree<T>::InsertRecursively(TreeNode*& parent, TreeNode* node) {
    if (!parent) {
        parent = node;
        node->SetRed();
        InsertFixUp(node);
        return true;
    }

    if (*node < *parent) {
        if (parent->left_) {
            return InsertRecursively(parent->left_, node);
        }

        node->parent_ = parent;
        parent->left_ = node;
        node->SetRed();
        InsertFixUp(node);
        return true;
    } else if (*node > *parent) {
        if (parent->right_) {
            return InsertRecursively(parent->right_, node);
        }

        node->parent_ = parent;
        parent->right_ = node;
        node->SetRed();
        InsertFixUp(node);
        return true;
    }
    return false;
}

template<typename T>
typename RBTree<T>::TreeNode*
RBTree<T>::SearchRecursively(TreeNode* node, const T& target) const {
    if (!node) return nullptr;
    if (*node == target) return node;
    else if (*node > target) {
        return SearchRecursively(node->left_, target);
    } else {
        return SearchRecursively(node->right_, target);
    }

    return nullptr;
}

template<typename T>
bool RBTree<T>::DeleteRecursively(TreeNode* node, const T& target) {
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
            // Case 1: no children
            // Delete Directly
            delete node;
            if (is_root) BaseTreeType::root_ = nullptr;
            else {
                if (is_left_child) parent->left_ = nullptr;
                else parent->right_ = nullptr;
            }

        } else if (!node->right_) {
            // Case 2: has one child
            // Replace node with the child
            TreeNode *l_node = node->left_;
            if (is_root) {
                BaseTreeType::root_ = l_node;
            } else {
                if (is_left_child) parent->left_ = l_node;
                else parent->right_ = l_node;
            }
            l_node->parent_ = parent;
            delete node;

        } else if (!node->left_) {
            // Case 2
            TreeNode *r_node = node->right_;
            if (is_root) {
                BaseTreeType::root_ = r_node;
            } else {
                if (is_left_child) parent->left_ = r_node;
                else parent->right_ = r_node;
            }
            r_node->parent_ = parent;
            delete node;

        } else {
            // Case 3: has 2 children
            // Find the inorder proceeding node
            TreeNode *ino_prev = node->left_;
            while (ino_prev->right_) {
                ino_prev = ino_prev->right_;
            }
            // Exchange the value of node and proceeding node
            node->data_ = ino_prev->data_;

            // Delete proceeding
            return DeleteRecursively(ino_prev, ino_prev->data_);
        }

        return true;
    }
}

template<typename T>
void RBTree<T>::LeftRotate(TreeNode* node) {
    /*      parent               parent
     *        |                    |
     *        P                    V
     *      /   \    ------\     /   \
     *     F     V   ------/    P     X
     *         /   \          /   \
     *        R     X        F     R
     */
    TreeNode* v_node = node->right_;
    TreeNode* r_node = v_node->left_;
    TreeNode* parent = node->parent_;
    node->right_ = r_node;

    if (r_node) {
        r_node->parent_ = node;
    }

    v_node->parent_ = parent;

    if (!parent) {
        BaseTreeType::root_ = v_node;
    } else {
        if (parent->left_ == node) {
            parent->left_ = v_node;
        } else {
            parent->right_ = v_node;
        }
    }

    v_node->left_ = node;
    node->parent_ = v_node;
}

template<typename T>
void RBTree<T>::RightRotate(TreeNode* node) {
    /*      parent               parent
     *        |                    |
     *        P                    F
     *      /   \    ------\     /   \
     *     F     V   ------/    D     P
     *   /   \                      /   \
     *  D     K                    K     V
     */
    TreeNode* f_node = node->left_;
    TreeNode* k_node = f_node->right_;
    TreeNode* parent = node->parent_;
    node->left_ = k_node;

    if (k_node) {
        k_node->parent_ = node;
    }

    f_node->parent_ = parent;
    if (!parent) {
        BaseTreeType::root_ = f_node;
    } else {
        if (parent->left_ == node) {
            parent->left_ = f_node;
        } else {
            parent->right_ = f_node;
        }
    }

    f_node->right_ = node;
    node->parent_ = f_node;
}


template<typename T>
void RBTree<T>::InsertFixUp(TreeNode*& node) {
    TreeNode *parent = node->parent_;
    if (parent == nullptr) {
        // Case 1: Node is root, set it to black;
        node->SetBlack();

    } else if (parent->IsRed()) {
        // Case 2: Parent node is red
        // There must be a grand parent
        TreeNode* grand_parent = parent->parent_;
        bool parent_is_left = (parent == grand_parent->left_);
        bool is_left = (node == parent->left_);
        TreeNode *uncle = (parent_is_left ? grand_parent->right_ : grand_parent->left_);

        if (uncle && uncle->IsRed()) {
            // Case 2.1: Uncle node is red
            // Set parent & uncle to black, set grand parent to red
            parent->SetBlack();
            uncle->SetBlack();
            grand_parent->SetRed();

            // Fixup grandparent recursively
            InsertFixUp(grand_parent);
        } else {
            // Case 2.2: Uncle is black
            bool has_rotate = false;
            if (parent_is_left) {
                if (!is_left) {
                    // node is right child, parent is left
                    // left rotate parent
                    LeftRotate(parent);
                    has_rotate = true;
                }
            } else {
                if (is_left) {
                    // node is left child, parent is right
                    // right rotate parent
                    RightRotate(parent);
                    has_rotate = true;
                }
            }

            if (has_rotate) {
                InsertFixUp(parent);
            } else {
                // Case 2.3: Uncle is black, node is left child
                // Set parent to black, set grand parent to red
                parent->SetBlack();
                grand_parent->SetRed();

                // rotate grand parent
                if (parent_is_left) {
                    RightRotate(grand_parent);
                } else {
                    LeftRotate(grand_parent);
                }
            }
        }
    } 

    // Case 3: Parent node is black, do nothing
    return;
}

// ------------ AVL Tree -------------

template<typename T>
class AVLTree final : public BinaryTreeBase<T> {
 public:
    AVLTree() {}
    ~AVLTree() {}

 protected:
    using TreeNode = typename BinaryTreeBase<T>::TreeNodeType;
    using BaseTreeType = BinaryTreeBase<T>;

    bool InsertRecursively(TreeNode*& parent, TreeNode* node) override;
    TreeNode* SearchRecursively(TreeNode* node, const T& target) const override;
    bool DeleteRecursively(TreeNode* node, const T& target) override;

};

template<typename T>
bool AVLTree<T>::InsertRecursively(TreeNode*& parent, TreeNode* node) {
    return false;
}

template<typename T>
typename AVLTree<T>::TreeNode*
AVLTree<T>::SearchRecursively(TreeNode* node, const T& target) const {
    return nullptr;
}

template<typename T>
bool AVLTree<T>::DeleteRecursively(TreeNode* node, const T& target) {
    return false;
}

}  // namespace binary_tree

#endif