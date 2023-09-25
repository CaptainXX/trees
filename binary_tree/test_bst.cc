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
    RBTree<int> rbt;
}

} // namespace ds_practice

int main() {
    ds_practice::TestBinarySearchTree();
    return 0;
}