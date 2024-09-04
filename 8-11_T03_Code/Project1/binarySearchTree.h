#pragma once
#include "SearchTreeNode.h"
template <typename T>
class BST
{
private:
    BSTNode<T>* root;

public:
    BST() {
        root = nullptr;
    }

    ~BST() {
        deleteTree(root);
    }

    void insert(T value) {
        root = insertHelper(root, value);
    }

    void remove(T value) {
        root = removeHelper(root, value);
    }

    bool search(T value) {
        return searchHelper(root, value);
    }

    void printInOrder() {
        printInOrderHelper(root);
    }

private:
    BSTNode<T>* insertHelper(BSTNode<T>* node, T value) {
        if (node == nullptr) {
            node = new BSTNode<T>(value);
        }
        else if (value < node->data) {
            node->left = insertHelper(node->left, value);
        }
        else if (value > node->data) {
            node->right = insertHelper(node->right, value);
        }

        return node;
    }

    BSTNode<T>* removeHelper(BSTNode<T>* node, T value) {
        if (node == nullptr) {
            return nullptr;
        }
        else if (value < node->data) {
            node->left = removeHelper(node->left, value);
        }
        else if (value > node->data) {
            node->right = removeHelper(node->right, value);
        }
        else {
            // Case 1: Node is a leaf node
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            }
            // Case 2: Node has only one child
            else if (node->left == nullptr) {
                BSTNode<T>* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr) {
                BSTNode<T>* temp = node;
                node = node->left;
                delete temp;
            }
            // Case 3: Node has two children
            else {
                BSTNode<T>* temp = findMin(node->right);
                node->data = temp->data;
                node->right = removeHelper(node->right, temp->data);
            }
        }

        return node;
    }

    BSTNode<T>* findMin(BSTNode<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    bool searchHelper(BSTNode<T>* node, T value) {
        if (node == nullptr) {
            return false;
        }
        else if (value == node->data) {
            return true;
        }
        else if (value < node->data) {
            return searchHelper(node->left, value);
        }
        else {
            return searchHelper(node->right, value);
        }
    }

    void printInOrderHelper(BSTNode<T>* node) {
        if (node != nullptr) {
            printInOrderHelper(node->left);
            cout << node->data << " ";
            printInOrderHelper(node->right);
        }
    }

    void deleteTree(BSTNode<T>* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};