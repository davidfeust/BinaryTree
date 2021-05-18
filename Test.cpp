//
// Created by david on 14/05/2021.
//

#include <ostream>
#include <set>

#include "BinaryTree.hpp"
#include "doctest.h"

using namespace ariel;
using namespace std;

void create_tree();

void create_tree(int n, int height);

TEST_CASE ("print") {
    BinaryTree<int> bt;
    bt.add_root(0);
    for (int i = 1; i < 50; ++i) {
        int r = rand() % 2;
        int prev = (rand() % i);
        try {
            if (r == 1) {
                bt.add_right(prev, i);
            } else {
                bt.add_left(prev, i);
            }
        } catch (exception &e) {
            continue;
        }
    }
    cout << bt << endl;
}

class MyInt {
    int x;

public:
    MyInt(int x) : x(x) {}

    bool operator==(const MyInt &rhs) const { return x == rhs.x; }

    friend ostream &operator<<(ostream &os, const MyInt &anInt) {
        return (os << anInt.x);
    }
};

TEST_CASE ("add_root") {
    for (int i = 0; i < 20; ++i) {
        BinaryTree<int> bt;
        int r = rand() % 250;
                CHECK_NOTHROW(bt.add_root(r));
                CHECK_EQ(*bt.begin(), r);
                CHECK_EQ(*bt.begin_inorder(), r);
                CHECK_EQ(*bt.begin_postorder(), r);
                CHECK_EQ(*bt.begin_preorder(), r);
    }

    for (int i = 0; i < 20; ++i) {
        BinaryTree<string> bt;
        int n = rand() % 250;
        string r = to_string(n);
                CHECK_NOTHROW(bt.add_root(r));
                CHECK_EQ(*bt.begin(), r);
                CHECK_EQ(*bt.begin_inorder(), r);
                CHECK_EQ(*bt.begin_postorder(), r);
                CHECK_EQ(*bt.begin_preorder(), r);
    }
    for (int i = 0; i < 20; ++i) {
        BinaryTree<MyInt> bt;
        int n = rand() % 250;
        auto r = MyInt(n);
                CHECK_NOTHROW(bt.add_root(MyInt(n)));
                CHECK_EQ(*bt.begin(), r);
                CHECK_EQ(*bt.begin_inorder(), r);
                CHECK_EQ(*bt.begin_postorder(), r);
                CHECK_EQ(*bt.begin_preorder(), r);
    }
}

TEST_CASE ("add_left") {
    for (int i = 0; i < 10; ++i) {
        BinaryTree<int> bt;
        set<int> set;
        while (set.size() <= 20) {
            int n = rand() % 250;
            set.insert(n);
        }
        int prev = -1;
        for (auto i = set.begin(); i != set.end(); ++i) {
            if (i == set.begin()) {
                        CHECK_NOTHROW(bt.add_root(*i));
                prev = *i;
            } else {
                        CHECK_NOTHROW(bt.add_left(prev, *i));
                prev = *i;
            }
        }
        for (auto[i, j] = tuple{bt.begin_preorder(), set.begin()};
             i != bt.end_preorder() && j != set.end(); ++i, ++j) {
                    CHECK_EQ(*i, *j);
        }

        for (auto[i, j, k] =
             tuple{bt.begin_postorder(), bt.begin_inorder(), set.rbegin()};
             i != bt.end_postorder() && j != bt.end_inorder() &&
             k != set.rend();
             ++i, ++k, ++j) {
                    CHECK_EQ(*i, *k);
                    CHECK_EQ(*j, *k);
        }
    }
}

TEST_CASE ("add_right") {
    for (int i = 0; i < 10; ++i) {
        BinaryTree<int> bt;
        set<int> set;
        while (set.size() <= 20) {
            int n = rand() % 250;
            set.insert(n);
        }
        int prev = -1;
        for (auto i = set.begin(); i != set.end(); ++i) {
            if (i == set.begin()) {
                        CHECK_NOTHROW(bt.add_root(*i));
                prev = *i;
            } else {
                        CHECK_NOTHROW(bt.add_right(prev, *i));
                prev = *i;
            }
        }
        for (auto[i, j] = tuple{bt.begin_postorder(), set.rbegin()};
             i != bt.end_postorder() && j != set.rend(); ++i, ++j) {
                    CHECK_EQ(*i, *j);
        }

        for (auto[i, j, k] =
             tuple{bt.begin_preorder(), bt.begin_inorder(), set.begin()};
             i != bt.end_preorder() && j != bt.end_inorder() && k != set.end();
             ++i, ++k, ++j) {
                    CHECK_EQ(*i, *k);
                    CHECK_EQ(*j, *k);
        }
    }
}

//BinaryTree<int> &create_tree(int n, int height, BinaryTree<int> &bt) {
//    if (height == 0) {
//        return bt;
//    }
//    bt.add_left(n, )
//}

//void create_tree() {
//    BinaryTree<int> bt;
//    bt.add_root(0);
//    create_tree(0, 3);
//}
//void create_tree() {
//    BinaryTree<int> bt;
//    set<int> set;
//    while (set.size() <= 20) {
//        int n = rand() % 250;
//        set.insert(n);
//    }
//    int prev = -1;
//    bool right = true;
//    for (auto i = set.begin(); i != set.end(); ++i) {
//        if (i == set.begin()) {
//            bt.add_root(*i);
//            prev = *i;
//        } else {
//            if (right) {
//                bt.add_right(prev, *i);
//                right = false;
//            } else {
//                bt.add_left(prev, *i);
//                right = true;
//                prev = *i;
//            }
//        }
//    }
//    cout << bt;
//}

