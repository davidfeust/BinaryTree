//
// Created by david on 14/05/2021.
//

#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <memory>

namespace ariel {

    template<typename T>
    class BinaryTree {

    private:
        struct Node {
            T value;
            std::shared_ptr<Node> right, left;

            Node(const T &value) :
                    value(value), right(nullptr), left(nullptr) {}

            Node(const Node &other) : value(other.value) {
                if (other.left != nullptr) {
                    std::shared_ptr<Node> tmp{new Node(*other.left)};
                    left = tmp;
                } else {
                    left = nullptr;
                }
                if (other.right != nullptr) {
                    std::shared_ptr<Node> tmp{new Node(*other.right)};
                    right = tmp;
                } else {
                    right = nullptr;
                }
            }

            Node &operator=(const Node &other) {
                if (this == &other) {
                    return *this;
                }
                value = other.value;
                if (other.left != nullptr) {
                    left = new Node(*other.left);
                } else {
                    left = nullptr;
                }
                if (other.right != nullptr) {
                    right = new Node(*other.right);
                } else {
                    right = nullptr;
                }
            }

            Node(Node &&other) noexcept {
                value = other.value;
                right = other.right;
                other.right = nullptr;
                left = other.left;
                other.left = nullptr;
            }

            Node &operator=(Node &&other) noexcept {
                if (this == &other) {
                    return *this;
                }
                value = other.value;
                right = other.right;
                other.right = nullptr;
                left = other.left;
                other.left = nullptr;
            }

            ~Node() = default;
        }; // END Node class

        static void fill_preorder(std::shared_ptr<Node> *n, std::vector<std::shared_ptr<Node>> &vector) {
            if (*n == nullptr) {
                return;
            }
            vector.push_back(*n);
            fill_preorder(&(*n)->left, vector);
            fill_preorder(&(*n)->right, vector);
        }

        static void fill_inorder(std::shared_ptr<Node> *n, std::vector<std::shared_ptr<Node>> &vector) {
            if (*n == nullptr) {
                return;
            }
            fill_inorder(&(*n)->left, vector);
            vector.push_back(*n);
            fill_inorder(&(*n)->right, vector);
        }

        static void fill_postorder(std::shared_ptr<Node> *n, std::vector<std::shared_ptr<Node>> &vector) {
            if (*n == nullptr) {
                return;
            }
            fill_postorder(&(*n)->left, vector);
            fill_postorder(&(*n)->right, vector);
            vector.push_back(*n);
        }

        std::shared_ptr<Node> root;
        uint size;
        size_t len;

        /**
         * Search the value in the tree, start from n node.
         * @param n node to start the search on.
         * @param value value to search.
         * @return Node* of the node contain the value, nullptr if the value not in
         * the tree.
         */
        std::shared_ptr<Node> search(std::shared_ptr<Node> n, T value) {
            if (n == nullptr) {
                return nullptr;
            }
            if (n->value == value) {
                return n;
            }
            std::shared_ptr<Node> l = search(n->left, value);
            std::shared_ptr<Node> r = search(n->right, value);
            if (l == nullptr) {
                return r;
            }
            return l;
        }

        // Base on: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
        static void print(const std::string &prefix, const std::shared_ptr<Node> node, bool isRight, std::ostream &os,
                          size_t spaces) {
            if (node == nullptr) {
                return;
            }
            std::string spaces_str(spaces - 1, ' ');
            os << prefix << spaces_str;
            if (isRight) {
                os << "╙──";
            } else {
                os << "╠──";
            }

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

        static size_t calc_len(const std::shared_ptr<Node> n, size_t ans) {
            if (n == nullptr) {
                return 0;
            }
            std::stringstream stream;
            stream << n->value;
            ans = stream.str().size();
            size_t left_ans = calc_len(n->left, ans);
            size_t right_ans = calc_len(n->right, ans);
            if (left_ans > ans) { ans = left_ans; }
            if (right_ans > ans) { ans = right_ans; }
            return ans;
        }


    public:
        BinaryTree() : root(nullptr), size(0), len(0) {}

        BinaryTree(const BinaryTree &other) : len(other.len), size(other.size) {
            std::shared_ptr<Node> tmp{new Node(*other.root)};
            root = tmp;
        }

        BinaryTree(BinaryTree &&other) noexcept {
            size = other.size;
            len = other.size;
            root = other.root;
            other.root = nullptr;
        }

        ~BinaryTree() = default;

        BinaryTree &operator=(const BinaryTree &other) {
            if (this == &other) {
                return *this;
            }
            std::shared_ptr<Node> tmp{new Node(*other.root)};
            root = tmp;
            return *this;
        }

        BinaryTree &operator=(BinaryTree &&other) noexcept {
            if (this == &other) {
                return *this;
            }
            size = other.size;
            len = other.size;
            root = other.root;
            other.root = nullptr;
            return *this;
        }

        BinaryTree &add_root(T value) {
            if (root == nullptr) {
                std::shared_ptr<Node> tmp{new Node{value}};
                root = tmp;
                ++size;
            } else {
                root->value = value;
            }
            return *this;
        }

        BinaryTree &add_left(T existing_value, T new_value) {
            std::shared_ptr<Node> n = search(root, existing_value);
            if (n == nullptr) {
                throw std::runtime_error(
                        "Error: existing_value is no exist in the tree.\n");
            }
            if (n->left == nullptr) {
                std::shared_ptr<Node> tmp{new Node{new_value}};
                n->left = tmp;
                ++size;
            } else {
                n->left->value = new_value;
            }
            return *this;
        }

        BinaryTree &add_right(T existing_value, T new_value) {
            std::shared_ptr<Node> n = search(root, existing_value);
            if (n == nullptr) {
                throw std::runtime_error(
                        "Error: existing_value is no exist in the tree.\n");
            }
            if (n->right == nullptr) {
                std::shared_ptr<Node> tmp{new Node{new_value}};
                n->right = tmp;
                ++size;
            } else {
                n->right->value = new_value;
            }
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &tree) {
            os << "BinaryTree: (size = " << tree.size << ")" << std::endl;
            size_t len = calc_len(tree.root, 0);
            std::string spaces_str(len - 1, ' ');
            os << spaces_str << "╗" << std::endl;
            print("", tree.root, true, os, len);
            return os;
        }

        struct Iterator {

        private:
            std::shared_ptr<Node> curr;
            uint i;
            int type;  // -1: pre | 0: in | 1: post
            std::vector<std::shared_ptr<Node>> vector;

        public:

            Iterator(std::shared_ptr<Node> n, int flag) : curr(n), i(0), type(flag) {
                if (flag == -1) {
                    fill_preorder(&n, vector);
                } else if (flag == 0) {
                    fill_inorder(&n, vector);
                } else {
                    fill_postorder(&n, vector);
                }
                vector.push_back(nullptr);
                std::shared_ptr<Node> tmp{vector[0]};
                curr = tmp;
            }

            T &operator*() const { return curr->value; }

            T *operator->() const { return &(curr->value); }

            Iterator &operator++() {
                curr = vector[++i];
                return *this;
            }

            Iterator operator++(int) {
                Iterator temp = *this;
                curr = vector[++i];
                return temp;
            }

            bool operator==(const Iterator &rhs) const {
                return curr == rhs.curr;
            }

            bool operator!=(const Iterator &rhs) const {
                return curr != rhs.curr;
            }
        };  // END Iterator class

        Iterator begin_preorder() { return Iterator{root, -1}; }

        Iterator end_preorder() { return Iterator{nullptr, -1}; }

        Iterator begin_inorder() { return Iterator{root, 0}; }

        Iterator end_inorder() { return Iterator{nullptr, 0}; }

        Iterator begin_postorder() { return Iterator{root, 1}; }

        Iterator end_postorder() { return Iterator{nullptr, 1}; }

        Iterator begin() { return begin_inorder(); }

        Iterator end() { return end_inorder(); }

//        ConstIterator cbegin() const { return ConstIterator{root, 0}; }
//        ConstIterator cend() const { return ConstIterator{nullptr, 0}; }

    };
}  // namespace ariel