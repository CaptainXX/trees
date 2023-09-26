#include <vector>
#include <iostream>

#include "bst.hpp"

namespace ds_practice {

void TestBinarySearchTree() {
    srand(time(nullptr));
    BinarySearchTree<int> bst;

    int n_data = 100;
    std::vector<int> data_to_ins(n_data);
    // std::vector<int> data = {49, 49, 45, 49, 25, 65, 41, 13, 31, 58};
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

} // namespace ds_practice

int main() {
    // ds_practice::TestBinarySearchTree();
    ds_practice::TestRBTree();
    return 0;
}