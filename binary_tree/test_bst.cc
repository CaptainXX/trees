#include <vector>
#include <unordered_set>
#include <iostream>
#include <chrono>
#include <set>

#include <gtest/gtest.h>

#include "bt.hpp"

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> start_;
    int64_t &duration_us_;

public:
    explicit Timer(int64_t &dur) : duration_us_(dur) {
        start_ = std::chrono::steady_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        duration_us_ = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();
    }
};

class BstTest : public ::testing::Test {
 protected:
    void SetUp() override {
        data = std::move(std::vector<int> {49, 49, 45, 49, 25, 65, 41, 13, 31, 58});
        n_data = data.size();
        srand(kRandomSeed);
        for (int i = 0; i < kNPerfData; ++i) {
            perf_data[i] = rand() % kNPerfData;
        }
    }

    std::vector<int> data;

    static constexpr const int kNPerfData = 1000000;
    static constexpr const int kRandomSeed = 1234;
    int perf_data[kNPerfData];
    int n_data;

};

TEST_F(BstTest, Clear) {
    binary_tree::BinarySearchTree<int> bst;
    for (auto x : data) {
        bst.Insert(x);
    }
    EXPECT_NO_THROW(bst.Clear());
}

TEST_F(BstTest, GetHeight) {
    binary_tree::BinarySearchTree<int> bst;
    binary_tree::RBTree<int> rbt;
    for (auto x : data) {
        bst.Insert(x);
        rbt.Insert(x);
    }

    EXPECT_EQ(bst.GetHeight(), 5);
    EXPECT_EQ(rbt.GetHeight(), 4);
    bst.Clear();
    rbt.Clear();

    for (int i = 0; i < kNPerfData; ++i) {
        bst.Insert(perf_data[i]);
        rbt.Insert(perf_data[i]);
    }
    std::cout << "BST height with 1 million items: " << bst.GetHeight() << std::endl;
    std::cout << "RBT height with 1 million items: " << rbt.GetHeight() << std::endl;
}

TEST_F(BstTest, BSTreePrint) {
    binary_tree::BinarySearchTree<int> bst;
    for (auto x : data) {
        EXPECT_NO_THROW(bst.Insert(x));
    }

    std::cout << bst << std::endl;
    EXPECT_NO_THROW(bst.Clear());
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

    // Perf
    bst.Clear();
    int64_t bst_random_insertion_time = 0;
    {
        Timer _(bst_random_insertion_time);
        for (int i = 0; i < kNPerfData; ++i) {
            bst.Insert(perf_data[i]);
        }
    }
    std::cout << "BST insert " << kNPerfData << " items time: "
              << bst_random_insertion_time << " us" << std::endl;
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

    // Case: insert an ordered array
    bst.Clear();
    int samples = (kNPerfData > 100000 ? 100000 : kNPerfData);
    for (int i = 0; i < samples; ++i) {
        bst.Insert(i);
    }
    int64_t bst_ordered_search_time = 0;
    {
        Timer _(bst_ordered_search_time);
        for (int i = 0; i < samples; ++i) {
            bst.Search(i);
        }
    }
    std::cout << "BST search ordered " << samples << " times takes: "
              << bst_ordered_search_time << " us" << std::endl;

    // Perf
    bst.Clear();
    for (int i = 0; i < kNPerfData; ++i) {
        bst.Insert(perf_data[i]);
    }

    int64_t bst_random_search_time = 0;
    {
        Timer _(bst_random_search_time);
        for (int i = 0; i < kNPerfData; ++i) {
            bst.Search(perf_data[i]);
        }
    }
    std::cout << "BST search " << kNPerfData << " times takes: "
              << bst_random_search_time << " us" << std::endl;
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
    EXPECT_NO_THROW(rbt.Clear());
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
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 49
    ins = rbt.Insert(data[1]);
    EXPECT_EQ(ins, nullptr);
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 45
    ins = rbt.Insert(data[2]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 45);
    EXPECT_TRUE(rbt.IsTreeValid());
    
    // Insert 49
    ins = rbt.Insert(data[3]);
    EXPECT_EQ(ins, nullptr);
    EXPECT_TRUE(rbt.IsTreeValid());
    
    // Insert 25
    ins = rbt.Insert(data[4]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 25);
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 65
    ins = rbt.Insert(data[5]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 65);
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 41
    ins = rbt.Insert(data[6]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 41);
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 13
    ins = rbt.Insert(data[7]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 13);
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 31
    ins = rbt.Insert(data[8]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 31);
    EXPECT_TRUE(rbt.IsTreeValid());

    // Insert 58
    ins = rbt.Insert(data[9]);
    EXPECT_NE(ins, nullptr);
    EXPECT_EQ(*ins, 58);   
    EXPECT_TRUE(rbt.IsTreeValid());

    // Multi Insert test
    rbt.Clear();
    int samples = (kNPerfData > 10000 ? 10000 : kNPerfData);
    for (int i = 0; i < samples; ++i) {
        rbt.Insert(perf_data[i]);
        EXPECT_TRUE(rbt.IsTreeValid());
    }

    // Perf
    rbt.Clear();
    int64_t rbt_random_insertion_time = 0;
    {
        Timer _(rbt_random_insertion_time);
        for (int i = 0; i < kNPerfData; ++i) {
            rbt.Insert(perf_data[i]);
        }
    }
    std::cout << "RBT insert " << kNPerfData << " items time: "
              << rbt_random_insertion_time << " us" << std::endl;
}

TEST_F(BstTest, RBTreeSearch) {
    using Node = binary_tree::RBTree<int>::TreeNodeType;
    binary_tree::RBTree<int> rbt;
    Node *res = nullptr;

    for (auto x : data) {
        rbt.Insert(x);
    }

    res = rbt.Search(49);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 49);

    res = rbt.Search(45);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 45);

    res = rbt.Search(25);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 25);

    res = rbt.Search(65);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 65);

    res = rbt.Search(41);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 41);

    res = rbt.Search(13);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 13);

    res = rbt.Search(31);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 31);

    res = rbt.Search(58);
    EXPECT_NE(res, nullptr);
    EXPECT_EQ(*res, 58);

    res = rbt.Search(100);
    EXPECT_EQ(res, nullptr);

    // Case: insert an ordered array
    rbt.Clear();
    for (int i = 0; i < kNPerfData; ++i) {
        rbt.Insert(i);
    }
    int64_t bst_ordered_search_time = 0;
    {
        Timer _(bst_ordered_search_time);
        for (int i = 0; i < kNPerfData; ++i) {
            rbt.Search(i);
        }
    }
    std::cout << "RBT search ordered " << kNPerfData << " times takes: "
              << bst_ordered_search_time << " us" << std::endl;
    
    // Case: Compare to std::set
    std::set<int> stdlib_set;
    for (int i = 0; i < kNPerfData; ++i) {
        stdlib_set.insert(i);
    }
    int64_t set_ordered_search_time = 0;
    {
        Timer _(set_ordered_search_time);
        for (int i = 0; i < kNPerfData; ++i) {
            stdlib_set.find(i);
        }
    }
    std::cout << "std::set search ordered " << kNPerfData << " times takes: "
              << set_ordered_search_time << " us" << std::endl;
 

    // Perf
    rbt.Clear();
    for (int i = 0; i < kNPerfData; ++i) {
        rbt.Insert(perf_data[i]);
    }

    int64_t rbt_random_search_time = 0;
    {
        Timer _(rbt_random_search_time);
        for (int i = 0; i < kNPerfData; ++i) {
            rbt.Search(perf_data[i]);
        }
    }
    std::cout << "RBT search " << kNPerfData << " times takes: "
              << rbt_random_search_time << " us" << std::endl;
}

TEST_F(BstTest, RBTreeDelete) {
    using Node = binary_tree::RBTree<int>::TreeNodeType;
    binary_tree::RBTree<int> rbt;
    Node *res = nullptr;

    for (auto x : data) {
        rbt.Insert(x);
    }

    for (auto x : data) {
        rbt.Delete(x);

        EXPECT_TRUE(rbt.IsTreeValid());
    }

    // Multi Deletes
    rbt.Clear();
    int samples = (kNPerfData > 10000 ? 10000 : kNPerfData);
    for (int i = 0; i < samples; ++i) {
        rbt.Insert(perf_data[i]);
    }

    for (int i = 0; i < samples; ++i) {
        rbt.Delete(perf_data[i]);
        EXPECT_TRUE(rbt.IsTreeValid());
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}