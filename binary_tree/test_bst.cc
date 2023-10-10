#include <vector>
#include <unordered_set>
#include <iostream>

#include <gtest/gtest.h>

#include "bt.hpp"

class BstTest : public ::testing::Test {
 protected:
    void SetUp() override {
        data = std::move(std::vector<int> {49, 49, 45, 49, 25, 65, 41, 13, 31, 58});
        n_data = data.size();
    }

    std::vector<int> data;
    int n_data;

};

TEST_F(BstTest, BSTreePrint) {
    binary_tree::BinarySearchTree<int> bst;
    for (auto x : data) {
        bst.Insert(x);
    }

    std::cout << bst << std::endl;
}

TEST_F(BstTest, BSTreeInsert) {
    ASSERT_GE(n_data, 10);
    using Node = binary_tree::BinarySearchTree<int>::TreeNodeType;
    binary_tree::BinarySearchTree<int> bst;
    Node * ins = nullptr;

    // Insert 49
    ins = bst.Insert(data[0]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 49);

    // Insert 49
    ins = bst.Insert(data[1]);
    EXPECT_EQ(ins, nullptr);

    // Insert 45
    ins = bst.Insert(data[2]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 45);
    
    // Insert 49
    ins = bst.Insert(data[3]);
    EXPECT_EQ(ins, nullptr);
    
    // Insert 25
    ins = bst.Insert(data[4]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 25);

    // Insert 65
    ins = bst.Insert(data[5]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 65);

    // Insert 41
    ins = bst.Insert(data[6]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 41);

    // Insert 13
    ins = bst.Insert(data[7]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 13);

    // Insert 31
    ins = bst.Insert(data[8]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 31);

    // Insert 58
    ins = bst.Insert(data[9]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 58);
}

TEST_F(BstTest, BSTreeSearch) {
    using Node = binary_tree::BinarySearchTree<int>::TreeNodeType;
    binary_tree::BinarySearchTree<int> bst;
    Node *res = nullptr;

    for (auto x : data) {
        bst.Insert(x);
    }

    res = bst.Search(49);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 49);

    res = bst.Search(45);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 45);

    res = bst.Search(25);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 25);

    res = bst.Search(65);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 65);

    res = bst.Search(41);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 41);

    res = bst.Search(13);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 13);

    res = bst.Search(31);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 31);

    res = bst.Search(58);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 58);

    res = bst.Search(100);
    EXPECT_EQ(res, nullptr);

}

TEST_F(BstTest, BSTreeDelete) {
    ASSERT_GE(n_data, 10);
    using Node = binary_tree::BinarySearchTree<int>::TreeNodeType;
    binary_tree::BinarySearchTree<int> bst;
    std::unordered_set<int> deleted_data;
    bool res = false;

    for (auto x : data) {
        bst.Insert(x);
    }

    res = bst.Delete(data[0]);
    deleted_data.insert(data[0]);
    EXPECT_TRUE(res);

    res = bst.Delete(data[0]);
    EXPECT_FALSE(res);

    for (int i = 1; i < n_data; ++i) {
        res = bst.Delete(data[i]);
        if (deleted_data.find(data[i]) == deleted_data.end()) {
            EXPECT_TRUE(res);
            deleted_data.insert(data[i]);
        } else {
            EXPECT_FALSE(res);
        }
    }
}

TEST_F(BstTest, RBTreePrint) {
    binary_tree::RBTree<int> rbt;
    for (auto x : data) {
        auto ret = rbt.Insert(x);
    }

    std::cout << rbt << std::endl;
}

TEST_F(BstTest, RBTreeInsert) {
    ASSERT_GE(n_data, 10);
    using Node = binary_tree::RBTree<int>::TreeNodeType;
    binary_tree::RBTree<int> rbt;
    Node * ins = nullptr;

    // Insert 49
    ins = rbt.Insert(data[0]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 49);

    // Insert 49
    ins = rbt.Insert(data[1]);
    EXPECT_EQ(ins, nullptr);

    // Insert 45
    ins = rbt.Insert(data[2]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 45);
    
    // Insert 49
    ins = rbt.Insert(data[3]);
    EXPECT_EQ(ins, nullptr);
    
    // Insert 25
    ins = rbt.Insert(data[4]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 25);

    // Insert 65
    ins = rbt.Insert(data[5]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 65);

    // Insert 41
    ins = rbt.Insert(data[6]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 41);

    // Insert 13
    ins = rbt.Insert(data[7]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 13);

    // Insert 31
    ins = rbt.Insert(data[8]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 31);

    // Insert 58
    ins = rbt.Insert(data[9]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 58);   
}

namespace binary_tree {

void TestBinarySearchTree() {
    srand(time(nullptr));
    BinarySearchTree<int> bst;

    int n_data = 100;
    std::vector<int> data_to_ins(n_data);
    for (int i = 0; i < n_data; ++i) {
        data_to_ins[i] = (rand() % (10 * n_data));
        // data_to_ins[i] = data[i];
    }

    std::cout << "Insert data: ";
    for (auto x : data_to_ins) {
        std::cout << x << " ";
        if (bst.Insert(x)) {
            std::cout << "ok, ";
        }
    }
    std::cout << std::endl;

    std::cout << "bst: " << bst;

    std::cout << "Search exist: " << *bst.Search(data_to_ins[rand() % data_to_ins.size()]) << std::endl;
    std::cout << "Search non-exist: " << (bst.Search(-1) == nullptr ? "Not found" : "found") << std::endl;

    for (auto x : data_to_ins) {
        int to_del = x;
        if (bst.Delete(to_del)) {
            std::cout << "Delete " << to_del << " success\n";
        } else {
            std::cout << "Delete " << to_del << " failed\n";
        }
        std::cout << "bst after delete: " << bst;
    }
}

void TestRBTree() {
    srand(time(nullptr));
    RBTree<int> rbt;

    int n_data = 10;
    std::vector<int> data_to_ins(n_data);
    std::vector<int> data = {49, 49, 45, 49, 25, 65, 41, 13, 31, 58};
    for (int i = 0; i < n_data; ++i) {
        // data_to_ins[i] = (rand() % (10 * n_data));
        data_to_ins[i] = data[i];
    }

    std::cout << "Insert data: ";
    for (auto x : data_to_ins) {
        std::cout << x << " ";
        if (rbt.Insert(x)) {
            std::cout << "ok, ";
        }
    }
    std::cout << std::endl;

    std::cout << "rbt: " << rbt;

    int rot_test_item = 25;
    auto node = rbt.Search(rot_test_item);
    if (node) {
        std::cout << "Search for " << rot_test_item << ": " << *node << std::endl;
        rbt.TestRotate(true, node);
        std::cout << "Tree after left rotate " << rot_test_item << ": " << rbt << std::endl;
        rbt.TestRotate(false, node);
        std::cout << "Tree after right rotate " << rot_test_item << ": " << rbt << std::endl;
    } else {
        std::cout << "Search " << rot_test_item << " failed\n";
    }

    // std::cout << "Search exist: " << *rbt.Search(data_to_ins[rand() % data_to_ins.size()]) << std::endl;
    // std::cout << "Search non-exist: " << (rbt.Search(-1) == nullptr ? "Not found" : "found") << std::endl;

    // for (auto x : data_to_ins) {
    //     int to_del = x;
    //     if (rbt.Delete(to_del)) {
    //         std::cout << "Delete " << to_del << " success\n";
    //     } else {
    //         std::cout << "Delete " << to_del << " failed\n";
    //     }
    //     std::cout << "bst after delete: " << rbt;
    // }
}

} // namespace binary_tree 

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}