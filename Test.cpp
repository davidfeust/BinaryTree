//
// Created by david on 14/05/2021.
//

#include <ostream>
#include <set>
#include <tuple>

#include "BinaryTree.hpp"
#include "doctest.h"

using namespace ariel;
using namespace std;

void preorder(size_t index, vector<int> &tree, vector<int> &pre);

void inorder(size_t index, vector<int> &tree, vector<int> &in);

void postorder(size_t index, vector<int> &tree, vector<int> &post);

TEST_CASE("print") {
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

TEST_CASE("add_root") {
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

TEST_CASE("add_left") {
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
        for (auto [i, j] = tuple{bt.begin_preorder(), set.begin()};
             i != bt.end_preorder() && j != set.end(); ++i, ++j) {
            CHECK_EQ(*i, *j);
        }

        for (auto [i, j, k] =
                 tuple{bt.begin_postorder(), bt.begin_inorder(), set.rbegin()};
             i != bt.end_postorder() && j != bt.end_inorder() &&
             k != set.rend();
             ++i, ++k, ++j) {
            CHECK_EQ(*i, *k);
            CHECK_EQ(*j, *k);
        }
    }
}

TEST_CASE("add_right") {
    for (int t = 0; t < 10; ++t) {
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
        for (auto [i, j] = tuple{bt.begin_postorder(), set.rbegin()};
             i != bt.end_postorder() && j != set.rend(); ++i, ++j) {
            CHECK_EQ(*i, *j);
        }

        for (auto [i, j, k] =
                 tuple{bt.begin_preorder(), bt.begin_inorder(), set.begin()};
             i != bt.end_preorder() && j != bt.end_inorder() && k != set.end();
             ++i, ++k, ++j) {
            CHECK_EQ(*i, *k);
            CHECK_EQ(*j, *k);
        }
    }
}

TEST_CASE("Big tree") {
    BinaryTree<int> bt;
    bt.add_root(0);
    for (int i = 0; i < 5000; ++i) {
        if (i % 2 == 0) {
            bt.add_right(i, i + 1);
        } else {
            bt.add_left(i, i + 1);
        }
    }
}

TEST_CASE("Not existing values") {
    BinaryTree<int> bt;
    bt.add_root(0);
    // create tree with some nodes
    for (int i = 0; i < 15; ++i) {
        if (i % 2 == 0) {
            CHECK_NOTHROW(bt.add_right(i, i + 1));
        } else {
            CHECK_NOTHROW(bt.add_left(i, i + 1));
        }
    }
    // range of int not in the tree
    for (int i = 20; i < 40; ++i) {
        if (i % 2 == 0) {
            CHECK_THROWS(bt.add_right(i, i + 1));
        } else {
            CHECK_THROWS(bt.add_left(i, i + 1));
        }
    }
}

TEST_CASE("Iterators simple") {
    SUBCASE("==") {
        BinaryTree<int> bt;
        bt.add_root(0).add_left(0, 1);
        auto it1 = bt.begin();
        auto it2 = bt.begin_postorder();
        auto it3 = bt.begin_inorder();
        auto it4 = bt.begin_preorder();
        CHECK(it1 == it3);
        CHECK(it1 == it2);
        CHECK(it2 != it4);
    }
    SUBCASE("++") {
        BinaryTree<int> bt;
        bt.add_root(0).add_left(0, 1).add_right(0, 2);
        auto it1 = bt.begin();
        auto it2 = bt.begin_postorder();
        auto it3 = bt.begin_inorder();
        auto it4 = bt.begin_preorder();

        CHECK(*it1 == 1);
        CHECK(*it3 == 1);
        CHECK_NOTHROW(++it1);
        CHECK_NOTHROW(++it3);
        CHECK_EQ(*it1, 0);
        CHECK_EQ(*it3, 0);
        CHECK_NOTHROW(it1++);
        CHECK_NOTHROW(it3++);
        CHECK(*it1 == 2);
        CHECK(*it3 == 2);
    }
}

TEST_CASE("Iterators") {
    for (int k = 0; k < 30; ++k) {
        // create tree & vector tree:
        size_t size = rand() % 200;
        vector<int> tree;
        tree.reserve(size);
        for (int i = 0; i < size; ++i) {
            tree.push_back(i);
        }
        BinaryTree<int> bt;
        bt.add_root(0);
        for (size_t i = 0; i < size / 2; ++i) {
            if (2 * i + 1 < size) {
                bt.add_left((int)i, tree.at(2 * i + 1));
            }
            if (2 * i + 2 < size) {
                bt.add_right((int)i, tree.at(2 * i + 2));
            }
        }
        // fill orders vectors
        vector<int> pre, in, post;
        preorder(0, tree, pre);
        inorder(0, tree, in);
        postorder(0, tree, post);

        // checks all orders:
        for (auto [i, j] = tuple{bt.begin_preorder(), (size_t)0};
             i != bt.end_preorder(); ++i, ++j) {
            CHECK_EQ(*i, pre[j]);
        }
        for (auto [i, j] = tuple{bt.begin_inorder(), (size_t)0};
             i != bt.end_inorder(); ++i, ++j) {
            CHECK_EQ(*i, in[j]);
        }
        for (auto [i, j] = tuple{bt.begin_postorder(), (size_t)0};
             i != bt.end_postorder(); ++i, ++j) {
            CHECK_EQ(*i, post[j]);
        }
        size_t j = 0;
        for (int i : bt) {
            CHECK_EQ(i, in[j++]);
        }
    }
}

TEST_CASE("Copy") {
    BinaryTree<int> bt;
    bt.add_root(0).add_left(0, 1).add_right(0, 2).add_left(1, 3);
    cout << "bt: " << bt;
    BinaryTree<int> copy{bt};
    cout << "copy: " << copy;
    BinaryTree<int> bt1;
    bt1.add_root(10).add_left(10, 5);
    cout << "bt1: " << bt1;
    bt1 = bt;
    cout << "bt1: " << bt1;
}

void preorder(size_t index, vector<int> &tree, vector<int> &pre) {
    if (index >= 0 && index < tree.size()) {
        pre.push_back(tree[index]);
        preorder(index * 2 + 1, tree, pre);
        preorder(index * 2 + 2, tree, pre);
    }
}

void inorder(size_t index, vector<int> &tree, vector<int> &in) {
    if (index >= 0 && index < tree.size()) {
        inorder(index * 2 + 1, tree, in);
        in.push_back(tree[index]);
        inorder(index * 2 + 2, tree, in);
    }
}

void postorder(size_t index, vector<int> &tree, vector<int> &post) {
    if (index >= 0 && index < tree.size()) {
        postorder(index * 2 + 1, tree, post);
        postorder(index * 2 + 2, tree, post);
        post.push_back(tree[index]);
    }
}