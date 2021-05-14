//
// Created by david on 14/05/2021.
//

#pragma once

#include <iostream>
#include <ostream>
#include <vector>

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

        class Iterator {
        private:
            Node *curr;
            uint i;
            int type;  // -1: pre | 0: in | 1: post
            std::vector<Node *> vector;

            void fill_inorder(Node **n);

            void fill_preorder(Node **n);

            void fill_postorder(Node **n);

        public:
            Iterator(Node *n, uint size, int flag);

            T &operator*() const { return curr->value; }

            T *operator->() const { return &(curr->value); }

            Iterator &operator++() {
                curr = vector[++i];
                return *this;
            }

            const Iterator &operator++(int);

            bool operator==(const Iterator &rhs) const;

            bool operator!=(const Iterator &rhs) const;
        };  // END Iterator class

        Node *root;
        uint size;

        BinaryTree(const BinaryTree &other);

        BinaryTree &operator=(const BinaryTree &other);

        static void in_order(std::ostream &os, Node *n);

        Node *search(Node *n, T value);

    public:
        BinaryTree() : root(nullptr), size(0) {}

        ~BinaryTree() { delete root; }

        BinaryTree &add_root(T value);

        BinaryTree &add_left(T existing_value, T new_value);

        BinaryTree &add_right(T existing_value, T new_value);

        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &tree) {
            os << "BinaryTree:" << std::endl;
            in_order(os, tree.root);
            return os;
        }

        // iterators:
        Iterator begin_preorder() { return Iterator{root, size, -1}; }

        Iterator end_preorder() { return Iterator{nullptr, size, -1}; }

        Iterator begin_inorder() { return Iterator{root, size, 0}; }

        Iterator end_inorder() { return Iterator{nullptr, size, 0}; }

        Iterator begin_postorder() { return Iterator{root, size, 1}; }

        Iterator end_postorder() { return Iterator{nullptr, size, 1}; }

        Iterator begin() { return begin_inorder(); }

        Iterator end() { return end_inorder(); }

        const Iterator cbegin() { return begin_inorder(); }

        const Iterator cend() { return end_inorder(); }
    };

    /*
     * Public method in BinaryTree:
     */

    // TODO: Replace root- only value?
    template<typename T>
    BinaryTree<T> &BinaryTree<T>::add_root(T value) {
        root = new Node{value};
        ++size;
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
            ++size;
        } else {
            n->left->value = new_value;
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
            ++size;
        } else {
            n->right->value = new_value;
        }
        return *this;
    }

    /*
     * Private method in BinaryTree:
     */

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

    template<typename T>
    void BinaryTree<T>::in_order(std::ostream &os, BinaryTree::Node *n) {
        if (n == nullptr) {
            return;
        }
        in_order(os, n->left);
        os << n->value << " ";
        in_order(os, n->right);
    }

    /*
     * Iterator:
     */

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
    void BinaryTree<T>::Iterator::fill_inorder(BinaryTree::Node **n) {
        if (*n == nullptr) {
            return;
        }
        fill_inorder(&(*n)->left);
        vector.push_back(*n);
        fill_inorder(&(*n)->right);
    }

    template<typename T>
    void BinaryTree<T>::Iterator::fill_preorder(BinaryTree::Node **n) {
        if (*n == nullptr) {
            return;
        }
        vector.push_back(*n);
        fill_preorder(&(*n)->left);
        fill_preorder(&(*n)->right);
    }

    template<typename T>
    void BinaryTree<T>::Iterator::fill_postorder(BinaryTree::Node **n) {
        if (*n == nullptr) {
            return;
        }
        fill_postorder(&(*n)->left);
        fill_postorder(&(*n)->right);
        vector.push_back(*n);
    }

    template<typename T>
    BinaryTree<T>::Iterator::Iterator(BinaryTree::Node *n, uint size, int flag) : curr(n), i(0), type(flag) {
        if (flag == -1) {
            fill_preorder(&n);
        } else if (flag == 0) {
            fill_inorder(&n);
        } else {
            fill_postorder(&n);
        }
        vector.push_back(nullptr);
        curr = vector[0];
    }

    template<typename T>
    const typename BinaryTree<T>::Iterator &BinaryTree<T>::Iterator::operator++(int) {
        Iterator temp = *this;
        curr = vector[++i];
        return temp;
    }

}  // namespace ariel