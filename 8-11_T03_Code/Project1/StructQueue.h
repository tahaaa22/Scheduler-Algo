#pragma once
#include "StructNode.h"
#include <iostream>
using namespace std;

template <typename T>
class SQueue
{
private:
    SNode<T>* front;
    SNode<T>* back;
    int itemCount;

public:
    /*
    * Function: SQueue
    * The SQueue Constructor.
    */
    

    SQueue() : front(nullptr), back(nullptr), itemCount(0) {}

    /*
    * Function: ~SQueue
    * The SQueue Destructor.
    */
    ~SQueue()
    {
       // clear();
    }

    /*
    * Function: enqueue
    * Inserts a new node at the end of the queue.
    *
    * Parameters:
    *  - newNode: The node to be inserted in the queue.
    */
    void enqueue(SNode<T>* newNode)
    {
        if (!newNode)
        {
            return;
        }
        if (isEmpty())
        {
            front = back;
            front = newNode;
            back = newNode;
        }
        else
        {
            back->setNext(newNode);
            back = newNode;
        }
        if (newNode->getFirstItem() != 0)
        itemCount++;
    }

    /*
    * Function: dequeue
    * Removes the first element in the queue and returns its data as an SNode object.
    *
    * Returns:
    *  - SNode<T>: The first node's data as an SNode object.
    */
    SNode<T> dequeue()
    {
        SNode<T> emptyNode;
        if (isEmpty())
        {
            return emptyNode;
        }
        T value1 = front->getFirstItem();
        T value2 = front->getSecondItem();
        SNode<T>* delPtr = front;
        front = front->getNext();
      /*  if (!front) 
        {
            back = nullptr;
        }*/
        delPtr->setNext(nullptr);
        delete delPtr;
        delPtr = NULL;
        itemCount--;
        return SNode<T>(value1, value2);
    }

    void dequeueR()
    {
        //SNode<T> emptyNode;
        if (isEmpty())
        {
            return ;
        }
        SNode<T>* delPtr = front;
        front = front->getNext();
       /* if (!front)
        {
            back = nullptr;
        }*/
        delPtr->setNext(nullptr);
        delete delPtr;
        delPtr = NULL;
        itemCount--;
       
    }

    /*
    * Function: peek
    * Returns the data of the first element in the queue as an SNode object.
    *
    * Returns:
    *  - SNode<T>: The first node's data as an SNode object.
    */
    SNode<T>* peek() 
    {
        if (isEmpty())
        {
            return NULL;
        }
        T value1 = front->getFirstItem();
        T value2 = front->getSecondItem();
        SNode<T>* nod = new SNode <T> (value1, value2);
        return nod;
    }
    SNode<T>* peekR()
    {
        if (isEmpty())
        {
            return NULL ;
        }
        return front;
    }
    /*
     Function: size
     Returns the number of elements in the queue.

     Returns:
     - int: The number of elements in the queue.
    */
    int size() const
    {
        return itemCount;
    }

    /*
    * Function: isEmpty
    * Determines if the queue is empty or not.
    *
    * Returns:
    *  - bool: True if the queue is empty, False otherwise.
    */
    bool isEmpty() const
    {
        return itemCount == 0;
    }

    /*
    * Function: clear
    * Removes all the elements from the queue.
    */
    void clear()
    {
        while (!isEmpty()) {
            dequeue();
        }
    }
};
