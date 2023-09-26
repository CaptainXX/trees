#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <queue>
#include <vector>

namespace ds_practice {

#define CREATE_BASE_TREETYPE_MEMBERS(TYPE) \
    T data_; \
    TYPE *left_; \
    TYPE *right_; \
    TYPE *parent_; \

#define CREATE_OPERATORS_FOR_TYPE(TYPE) \
    inline bool operator<(const TYPE& rhs) { return this->data_ < rhs.data_; } \
    inline bool operator>(const TYPE& rhs) { return this->data_ > rhs.data_; } \
    inline bool operator==(const TYPE& rhs) { return this->data_ == rhs.data_; } \
    inline bool operator<(const T& rhs) { return this->data_ < rhs; } \
    inline bool operator>(const T& rhs) { return this->data_ > rhs; } \
    inline bool operator==(const T& rhs) { return this->data_ == rhs; } \
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
    TreeNode* Search(const T& target);
    bool Delete(const T& target);

    template<typename U, typename TreeNodeT>
    friend std::ostream& operator<<(std::ostream& os, const BinaryTreeBase<U, TreeNodeT>& bst);

 protected:
    TreeNode *root_;

    void Destroy(TreeNode* node);
    void InorderPrint(std::ostream& os, TreeNode* node) const;
    void TreePrint(std::ostream& os, TreeNode* node) const;

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
void BinaryTreeBase<T, TreeNode>::TreePrint(std::ostream& os, TreeNode* node) const {
    std::queue<TreeNode*> node_q;
    std::vector<std::vector<TreeNode*>> tree;

    node_q.push(node);
    while (!node_q.empty()) {
        int n_nodes = node_q.size();
        bool has_not_null = false;

        tree.push_back({});

        for (int i = 0; i < n_nodes; ++i) {
            TreeNode* top_node = node_q.front();
            node_q.pop();

            tree.back().push_back(top_node);

            if (top_node) {
                node_q.push(top_node->left_);
                node_q.push(top_node->right_);
                has_not_null = true;
            } else {
                node_q.push(nullptr);
                node_q.push(nullptr);
            }
        }

        if (!has_not_null) {
            tree.pop_back();
            break;
        }
    }

    int tree_depth = tree.size();
    for (int i = 0; i < tree_depth; ++i) {
        for (auto item : tree[i]) {
            if (item) {
                os << *item << " ";
            } else {
                os << "<x> ";
            }
        }
        os << std::endl;
    }
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
        return "(" + std::to_string(data_) + ", " + (color_ == Color::Red ? "Red" : "Black") + ")";
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
    TreeNode* SearchRecursively(TreeNode* node, const T& target) override;
    bool DeleteRecursively(TreeNode* node, const T& target) override;

    void LeftRotate(TreeNode* node);
    void RightRotate(TreeNode* node);

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
typename RBTree<T>::TreeNode*
RBTree<T>::SearchRecursively(TreeNode* node, const T& target) {
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