//
// Created by david on 14/05/2021.
//

#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include <iomanip>
#include <cstring>

namespace ariel {

    template<typename T>
    class BinaryTree {

    private:
        struct Node {
            T value;
            Node *right, *left;

            Node(const T &value) : value(value), right(nullptr), left(nullptr) {}

            ~Node() {
                delete left;
                delete right;
            }
        }; // END Node class

        static void fill_preorder(Node **n, std::vector<Node *> &vector);

        static void fill_inorder(Node **n, std::vector<Node *> &vector);

        static void fill_postorder(Node **n, std::vector<Node *> &vector);

        Node *root;
        uint size;
        size_t len;

        BinaryTree(const BinaryTree &other);

        BinaryTree &operator=(const BinaryTree &other);

        Node *search(Node *n, T value);

        static void print(const std::string &prefix, const Node *node, bool isRight, std::ostream &os, size_t spaces);

        void calc_len(T val);

    public:
        BinaryTree() : root(nullptr), size(0), len(0) {}

        ~BinaryTree() { delete root; }

        BinaryTree &add_root(T value);

        BinaryTree &add_left(T existing_value, T new_value);

        BinaryTree &add_right(T existing_value, T new_value);

        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &tree) {
            os << "BinaryTree: (size = " << tree.size << ")" << std::endl;
            std::string spaces_str(tree.len - 1, ' ');
            os << spaces_str << "╗" << std::endl;
            print("", tree.root, true, os, tree.len);
            return os;
        }

        // iterators:

        struct Iterator {

        private:
            Node *curr;
            uint i;
            int type;  // -1: pre | 0: in | 1: post
            std::vector<Node *> vector;

        public:

            Iterator(Node *n, int flag);

            T &operator*() const { return curr->value; }

            T *operator->() const { return &(curr->value); }

            Iterator &operator++();

            const Iterator &operator++(int);

            bool operator==(const Iterator &rhs) const;

            bool operator!=(const Iterator &rhs) const;
        };  // END Iterator class

        struct ConstIterator {

        private:
            Node *curr;
            uint i;
            int type;  // -1: pre | 0: in | 1: post
            std::vector<Node *> vector;

        public:
            ConstIterator(Node *n, int flag);

            const T &operator*() const { return curr->value; }

            const T *operator->() const { return &(curr->value); }

            ConstIterator &operator++();

            const ConstIterator &operator++(int);

            bool operator==(const ConstIterator &rhs) const;

            bool operator!=(const ConstIterator &rhs) const;
        };  // END ConstIterator class

        Iterator begin_preorder() { return Iterator{root, -1}; }

        Iterator end_preorder() { return Iterator{nullptr, -1}; }

        Iterator begin_inorder() { return Iterator{root, 0}; }

        Iterator end_inorder() { return Iterator{nullptr, 0}; }

        Iterator begin_postorder() { return Iterator{root, 1}; }

        Iterator end_postorder() { return Iterator{nullptr, 1}; }

        Iterator begin() { return begin_inorder(); }

        Iterator end() { return end_inorder(); }

        ConstIterator cbegin() const { return ConstIterator{root, 0}; }

        ConstIterator cend() const { return ConstIterator{nullptr, 0}; }
    };

    ////////////////////////////////
    //////  Implementations:  //////
    ////////////////////////////////

    /*
     * BinaryTree:
     */

    // TODO: Replace root- only value?
    template<typename T>
    BinaryTree<T> &BinaryTree<T>::add_root(T value) {
        root = new Node{value};
        ++size;
        calc_len(value);
        return *this;
    }

    template<typename T>
    BinaryTree<T> &BinaryTree<T>::add_left(T existing_value, T new_value) {
        Node *n = search(root, existing_value);
        if (n == nullptr) {
            throw std::runtime_error(
                    "Error: existing_value is no exist in the tree.\n");
        }
        if (n->left == nullptr) {
            n->left = new Node{new_value};
            calc_len(new_value);
            ++size;
        } else {
            n->left->value = new_value;
            calc_len(new_value);
        }
        return *this;
    }

    template<typename T>
    BinaryTree<T> &BinaryTree<T>::add_right(T existing_value, T new_value) {
        Node *n = search(root, existing_value);
        if (n == nullptr) {
            throw std::runtime_error(
                    "Error: existing_value is no exist in the tree.\n");
        }
        if (n->right == nullptr) {
            n->right = new Node{new_value};
            calc_len(new_value);
            ++size;
        } else {
            n->right->value = new_value;
            calc_len(new_value);
        }
        return *this;
    }

    /**
     * Search the value in the tree, start from n node.
     * @param n node to start the search on.
     * @param value value to search.
     * @return Node* of the node contain the value, nullptr if the value not in
     * the tree.
     */
    template<typename T>
    typename BinaryTree<T>::Node *BinaryTree<T>::search(BinaryTree::Node *n, T value) {
        if (n == nullptr) {
            return nullptr;
        }
        if (n->value == value) {
            return n;
        }
        Node *l = search(n->left, value);
        Node *r = search(n->right, value);
        if (l == nullptr) {
            return r;
        } else {
            return l;
        }
    }

    // Base on: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
    template<typename T>
    void BinaryTree<T>::print(const std::string &prefix, const BinaryTree::Node *node,
                              bool isRight, std::ostream &os, unsigned long spaces) {
        if (node == nullptr) {
            return;
        }
        std::string spaces_str(spaces - 1, ' ');
        os << prefix << spaces_str;
        os << (isRight ? "╙──" : "╠──");

        // print the value of the node
        os << std::setw((int) spaces) << node->value;
        if (node->left || node->right) {
            os << "╖";
        }
        os << std::endl;

        // enter the next tree level - left and right branch
        print(prefix + spaces_str + (isRight ? "    " : "║   "), node->right, false, os, spaces);
        print(prefix + spaces_str + (isRight ? "    " : "║   "), node->left, true, os, spaces);
    }

    template<typename T>
    void BinaryTree<T>::fill_inorder(BinaryTree::Node **n, std::vector<Node *> &v) {
        if (*n == nullptr) {
            return;
        }
        fill_inorder(&(*n)->left, v);
        v.push_back(*n);
        fill_inorder(&(*n)->right, v);
    }

    template<typename T>
    void BinaryTree<T>::fill_preorder(BinaryTree::Node **n, std::vector<Node *> &v) {
        if (*n == nullptr) {
            return;
        }
        v.push_back(*n);
        fill_preorder(&(*n)->left, v);
        fill_preorder(&(*n)->right, v);
    }

    template<typename T>
    void BinaryTree<T>::fill_postorder(BinaryTree::Node **n, std::vector<Node *> &v) {
        if (*n == nullptr) {
            return;
        }
        fill_postorder(&(*n)->left, v);
        fill_postorder(&(*n)->right, v);
        v.push_back(*n);
    }

    template<typename T>
    void BinaryTree<T>::calc_len(T val) {
        std::stringstream stream;
        stream << val;
        len = std::max(len, stream.str().size());
    }

    /*
     * Iterator:
     */

    template<typename T>
    BinaryTree<T>::Iterator::Iterator(BinaryTree::Node *n, int flag) : curr(n), i(0), type(flag) {
        if (flag == -1) {
            fill_preorder(&n, vector);
        } else if (flag == 0) {
            fill_inorder(&n, vector);
        } else {
            fill_postorder(&n, vector);
        }
        vector.push_back(nullptr);
        curr = vector[0];
    }

    template<typename T>
    bool ariel::BinaryTree<T>::Iterator::operator==(
            const ariel::BinaryTree<T>::Iterator &rhs) const {
        return curr == rhs.curr;
    }

    template<typename T>
    bool ariel::BinaryTree<T>::Iterator::operator!=(
            const ariel::BinaryTree<T>::Iterator &rhs) const {
        return curr != rhs.curr;
    }

    template<typename T>
    const typename BinaryTree<T>::Iterator &BinaryTree<T>::Iterator::operator++(int) {
        Iterator temp = *this;
        curr = vector[++i];
        return temp;
    }

    template<typename T>
    typename BinaryTree<T>::Iterator &BinaryTree<T>::Iterator::operator++() {
        curr = vector[++i];
        return *this;
    }

    /*
     * ConstIterator:
     */

    template<typename T>
    BinaryTree<T>::ConstIterator::ConstIterator(BinaryTree::Node *n, int flag) {
        std::cout << "@@@@@@@";
        if (flag == -1) {
            fill_preorder(&n, vector);
        } else if (flag == 0) {
            fill_inorder(&n, vector);
        } else {
            fill_postorder(&n, vector);
        }
        vector.push_back(nullptr);
        curr = vector[0];
    }

    template<typename T>
    bool ariel::BinaryTree<T>::ConstIterator::operator==(const ariel::BinaryTree<T>::ConstIterator &rhs) const {
        return curr == rhs.curr;
    }

    template<typename T>
    bool ariel::BinaryTree<T>::ConstIterator::operator!=(const ariel::BinaryTree<T>::ConstIterator &rhs) const {
        return curr != rhs.curr;
    }

    template<typename T>
    const typename BinaryTree<T>::ConstIterator &BinaryTree<T>::ConstIterator::operator++(int) {
        Iterator temp = *this;
        curr = vector[++i];
        return temp;
    }

    template<typename T>
    typename BinaryTree<T>::ConstIterator &BinaryTree<T>::ConstIterator::operator++() {
        curr = vector[++i];
        return *this;
    }

}  // namespace ariel