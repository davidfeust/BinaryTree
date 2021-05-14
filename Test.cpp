//
// Created by david on 14/05/2021.
//

#include "doctest.h"
#include "BinaryTree.hpp"

using namespace ariel;
using namespace std;

TEST_CASE ("0") {
    for (int i = 0; i < 30; ++i) {
                CHECK(1);
    }
    BinaryTree<int> bt;
    bt.add_root(0);
    for (int i = 1; i < 10; ++i) {
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

    for (int &i : bt) {
        cout << i << ", ";
    }
}