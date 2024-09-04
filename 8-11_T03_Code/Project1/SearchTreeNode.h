#pragma once
#include <iostream>
using namespace std;
template <typename T>
class BSTNode 
{
public:
    T data;
    BSTNode<T>* left;
    BSTNode<T>* right;

    BSTNode(T value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};